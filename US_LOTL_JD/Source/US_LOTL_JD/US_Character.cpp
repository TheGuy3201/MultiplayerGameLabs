// Fill out your copyright notice in the Description page of Project Settings.

#include "US_Character.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Engine/Engine.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/PawnNoiseEmitterComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/Controller.h"

#include "US_CharacterStats.h"
#include "Engine/DataTable.h"

#include "US_Interactable.h"
#include "Kismet/KismetSystemLibrary.h"

#include "US_WeaponProjectileComponent.h"

// Sets default values
AUS_Character::AUS_Character()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Camera BOOM
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	//Follow Camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	NoiseEmitter = CreateDefaultSubobject<UPawnNoiseEmitterComponent>(TEXT("NoiseEmitter"));
	NoiseEmitter->NoiseLifetime = 0.01f;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCapsuleComponent()->InitCapsuleSize(60.f, 96.0f);
	GetMesh()->SetRelativeLocation(FVector(0.0f, 0.0f, -91.0f));

	static ConstructorHelpers::FObjectFinder<USkeletalMesh>
		PlayerMesh(TEXT("/Game/KayKit/Characters/rogue"));
	if (PlayerMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(PlayerMesh.Object);
	}

	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->MaxWalkSpeed = 500.0f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.0f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.0f;

	Weapon = CreateDefaultSubobject<UUS_WeaponProjectileComponent>(TEXT("Weapon"));
	Weapon->SetupAttachment(RootComponent);
	Weapon->SetRelativeLocation(FVector(120.f, 70.f, 0.f));
	Weapon->SetIsReplicated(true);
}

// Called when the game starts or when spawned
void AUS_Character::BeginPlay()
{
	Super::BeginPlay();

	UpdateCharacterStats(1);

	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			if (!DefaultMappingContext)
			{
				// Try to load a default mapping context if not set by Blueprint
				static ConstructorHelpers::FObjectFinder<UInputMappingContext> DefaultIMC(TEXT("/Game/Input/IMC_Default.IMC_Default"));
				if (DefaultIMC.Succeeded())
				{
					DefaultMappingContext = DefaultIMC.Object;
				}
			}
			if (DefaultMappingContext)
			{
				Subsystem->AddMappingContext(DefaultMappingContext, 0);
			}
		}
	}
}

// Called every frame
void AUS_Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (GetLocalRole() != ROLE_Authority) return;
	
	FHitResult HitResult;
	FCollisionQueryParams QueryParams;
	QueryParams.bTraceComplex = true;
	QueryParams.AddIgnoredActor(this);
	auto SphereRadius = 50.f;
	auto StartLocation = GetActorLocation() + GetActorForwardVector() * 150.f;
	auto EndLocation = StartLocation + GetActorForwardVector() * 500.f;
	auto IsHit = UKismetSystemLibrary::SphereTraceSingle(
		GetWorld(), StartLocation, EndLocation, SphereRadius,
		UEngineTypes::ConvertToTraceType(ECC_WorldStatic),
		false, TArray<AActor*>(), EDrawDebugTrace::ForOneFrame, HitResult, true
	);

	if (IsHit && HitResult.GetActor()->GetClass()->ImplementsInterface(UUS_Interactable::StaticClass()))
	{
		DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, SphereRadius, 12, FColor::Magenta, false, 1.f);
		InteractableActor = HitResult.GetActor();
	}
	else
	{
		InteractableActor = nullptr;
	}

	if (GetCharacterMovement()->MaxWalkSpeed == GetCharacterStats()->SprintSpeed)
	{
		auto Noise = 1.f;
		if (GetCharacterStats() && GetCharacterStats()->StealthMultiplier)
		{
			Noise = Noise / GetCharacterStats()->StealthMultiplier;
		}
		NoiseEmitter->MakeNoise(this, Noise, GetActorLocation());
	}
}

// Called to bind functionality to input
void AUS_Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UE_LOG(LogTemp, Warning, TEXT("SetupPlayerInputComponent called on %s"), *GetName());

	// Log mapping and actions
	UE_LOG(LogTemp, Warning, TEXT("DefaultMappingContext: %s"), DefaultMappingContext ? *DefaultMappingContext->GetName() : TEXT("null"));
	UE_LOG(LogTemp, Warning, TEXT("MoveAction: %s, LookAction: %s, SprintAction: %s, InteractAction: %s"),
		MoveAction ? *MoveAction->GetName() : TEXT("null"),
		LookAction ? *LookAction->GetName() : TEXT("null"),
		SprintAction ? *SprintAction->GetName() : TEXT("null"),
		InteractAction ? *InteractAction->GetName() : TEXT("null"));

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		UE_LOG(LogTemp, Warning, TEXT("Using UEnhancedInputComponent for bindings"));

		if (MoveAction)
		{
			EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AUS_Character::Move);
			UE_LOG(LogTemp, Warning, TEXT("Bound MoveAction"));
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("MoveAction is null - cannot bind movement"));
		}

		if (LookAction)
		{
			EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AUS_Character::Look);
			UE_LOG(LogTemp, Warning, TEXT("Bound LookAction"));
		}

		if (SprintAction)
		{
			EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Started, this, &AUS_Character::SprintStart);
			EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &AUS_Character::SprintEnd);
			UE_LOG(LogTemp, Warning, TEXT("Bound SprintAction"));
		}

		if (InteractAction)
		{
			EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &AUS_Character::Interact);
			UE_LOG(LogTemp, Warning, TEXT("Bound InteractAction"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerInputComponent is NOT an UEnhancedInputComponent; got %s"), PlayerInputComponent ? *PlayerInputComponent->GetClass()->GetName() : TEXT("null"));
	}
}

void AUS_Character::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward direction
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right direction 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AUS_Character::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AUS_Character::SprintStart(const FInputActionValue& Value)
{
	SprintStart_Server();
}

void AUS_Character::SprintEnd(const FInputActionValue& Value)
{
	SprintEnd_Server();
}

void AUS_Character::Interact(const FInputActionValue& Value)
{
	Interact_Server();
}

void AUS_Character::Interact_Server_Implementation()
{
	if (InteractableActor)
	{
		IUS_Interactable::Execute_Interact(InteractableActor, this);
	}
}

void AUS_Character::UpdateCharacterStats(int32 CharacterLevel)
{
	auto IsSprinting = false;
	if (GetCharacterStats())
	{
		IsSprinting = GetCharacterMovement()->MaxWalkSpeed == GetCharacterStats()->SprintSpeed;
	}

	if (CharacterDataTable)
	{
		TArray<FUS_CharacterStats*> CharacterStatsRows;
		CharacterDataTable->GetAllRows<FUS_CharacterStats>(TEXT("US_Character"), CharacterStatsRows);
		if (CharacterStatsRows.Num() > 0)
		{
			const auto NewCharacterLevel = FMath::Clamp(CharacterLevel, 1, CharacterStatsRows.Num());
			CharacterStats = CharacterStatsRows[NewCharacterLevel - 1];
			GetCharacterMovement()->MaxWalkSpeed = CharacterStats->WalkSpeed;

			if (IsSprinting)
			{
				SprintStart_Server();
			}
		}
	}
}

void AUS_Character::SprintStart_Server_Implementation()
{
	SprintStart_Client();
}

void AUS_Character::SprintEnd_Server_Implementation()
{
	SprintEnd_Client();
}

void AUS_Character::SprintStart_Client_Implementation()
{
	if (GetCharacterStats())
	{
		GetCharacterMovement()->MaxWalkSpeed = GetCharacterStats()->SprintSpeed;
	}
}

void AUS_Character::SprintEnd_Client_Implementation()
{
	if (GetCharacterStats())
	{
		GetCharacterMovement()->MaxWalkSpeed = GetCharacterStats()->WalkSpeed;
	}
}