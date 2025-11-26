// Fill out your copyright notice in the Description page of Project Settings.


#include "US_PlayerController.h"
#include "US_Character.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputMappingContext.h"
#include "Engine/LocalPlayer.h"
#include "Engine/Engine.h"
#include "GameFramework/PlayerInput.h"
#include "InputAction.h"

AUS_PlayerController::AUS_PlayerController()
{
	// Enable input mode for UI and Game
	bShowMouseCursor = false;
	bEnableClickEvents = false;
	bEnableTouchEvents = true;
}

void AUS_PlayerController::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Log, TEXT("AUS_PlayerController::BeginPlay"));
}

void AUS_PlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (GEngine)
	{
		UE_LOG(LogTemp, Log, TEXT("AUS_PlayerController possessed: %s"), InPawn ? *InPawn->GetName() : TEXT("null"));
	}

	// Only register mapping on local player
	if (!IsLocalController())
	{
		return;
	}

	AUS_Character* Char = Cast<AUS_Character>(InPawn);
	if (!Char)
	{
		UE_LOG(LogTemp, Warning, TEXT("OnPossess: possessed pawn is not AUS_Character"));
		return;
	}

	// Ensure input focus is correct
	FInputModeGameOnly InputMode;
	SetInputMode(InputMode);
	bShowMouseCursor = false;

	// Enable input on the pawn (ensures InputComponent exists)
	Char->EnableInput(this);

	// Register mapping context on the local player subsystem
	if (ULocalPlayer* LP = GetLocalPlayer())
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LP))
		{
			if (UInputMappingContext* Ctx = Char->GetDefaultMappingContext())
			{
				Subsystem->AddMappingContext(Ctx, 0);
				UE_LOG(LogTemp, Log, TEXT("Added DefaultMappingContext '%s' to local subsystem"), *Ctx->GetName());
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Possessed character has no DefaultMappingContext assigned"));
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("No EnhancedInputLocalPlayerSubsystem found on LocalPlayer"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("GetLocalPlayer() returned null"));
	}

	// Ensure the pawn's InputComponent is an EnhancedInputComponent and bind actions directly as a fallback.
	if (UInputComponent* IC = Char->InputComponent)
	{
		if (UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(IC))
		{
			// Bind actions directly to the character to guarantee movement bindings
			if (Char->MoveAction)
			{
				EIC->BindAction(Char->MoveAction, ETriggerEvent::Triggered, Char, &AUS_Character::Move);
				UE_LOG(LogTemp, Log, TEXT("Directly bound MoveAction on possess."));
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("MoveAction is null on possessed character."));
			}

			if (Char->LookAction)
			{
				EIC->BindAction(Char->LookAction, ETriggerEvent::Triggered, Char, &AUS_Character::Look);
				UE_LOG(LogTemp, Log, TEXT("Directly bound LookAction on possess."));
			}

			if (Char->SprintAction)
			{
				EIC->BindAction(Char->SprintAction, ETriggerEvent::Started, Char, &AUS_Character::SprintStart);
				EIC->BindAction(Char->SprintAction, ETriggerEvent::Completed, Char, &AUS_Character::SprintEnd);
				UE_LOG(LogTemp, Log, TEXT("Directly bound SprintAction on possess."));
			}

			if (Char->InteractAction)
			{
				EIC->BindAction(Char->InteractAction, ETriggerEvent::Started, Char, &AUS_Character::Interact);
				UE_LOG(LogTemp, Log, TEXT("Directly bound InteractAction on possess."));
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Pawn InputComponent is not UEnhancedInputComponent (got %s)"), IC ? *IC->GetClass()->GetName() : TEXT("null"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Possessed pawn has no InputComponent at time of possess."));
	}
}

