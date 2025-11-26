// Fill out your copyright notice in the Description page of Project Settings.

#include "US_GameMode.h"
#include "US_GameState.h" 
#include "UObject/ConstructorHelpers.h"
#include "US_PlayerController.h"
#include "US_PlayerState.h"
#include "US_Character.h"
#include "US_Minion.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Engine.h"
#include "Engine/World.h"
#include "GameFramework/PlayerStart.h"
#include "UObject/SoftObjectPath.h"

AUS_GameMode::AUS_GameMode()
{
	// Set up game classes
	GameStateClass = AUS_GameState::StaticClass();
	PlayerStateClass = AUS_PlayerState::StaticClass();
	PlayerControllerClass = AUS_PlayerController::StaticClass();

	// Use the exact Blueprint reference path (without the _C suffix for ConstructorHelpers)
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/BP_Character.BP_Character"));

	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
		UE_LOG(LogTemp, Warning, TEXT("GameMode Constructor: DefaultPawnClass set to BP: %s"), *DefaultPawnClass->GetName());
	}
	else
	{
		// Fallback to the C++ class
		DefaultPawnClass = AUS_Character::StaticClass();
		UE_LOG(LogTemp, Warning, TEXT("GameMode Constructor: BP not found, DefaultPawnClass = %s"),
			DefaultPawnClass ? *DefaultPawnClass->GetName() : TEXT("NULL"));
	}
}

void AUS_GameMode::AlertMinions(AActor* AlertInstigator, float Radius, FVector Location)
{
	TArray<AActor*> Minions;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AUS_Minion::StaticClass(), Minions);
	for (const auto Minion : Minions)
	{
		if (AlertInstigator == Minion) continue;
		
		if (const auto Distance = FVector::Distance(AlertInstigator->GetActorLocation(), Minion->GetActorLocation()); Distance < Radius)
		{
			if (const auto MinionCharacter = Cast<AUS_Minion>(Minion))
			{
				MinionCharacter->GoToLocation(Location);
			}
		}
	}
}

void AUS_GameMode::StartPlay()
{
	Super::StartPlay();

	// If DefaultPawnClass still points to the C++ class, try a soft load
	if (DefaultPawnClass == AUS_Character::StaticClass())
	{
		// Use the _C suffix for runtime loading
		const FSoftClassPath BPPath(TEXT("/Game/BP_Character.BP_Character_C"));
		
		if (UClass* BPClass = BPPath.TryLoadClass<APawn>())
		{
			DefaultPawnClass = BPClass;
			UE_LOG(LogTemp, Warning, TEXT("StartPlay: Loaded BP pawn and set DefaultPawnClass -> %s"), *DefaultPawnClass->GetName());
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("StartPlay: Failed to load BP pawn at %s"), *BPPath.ToString());
		}
	}

	// Diagnostic logging (will appear in Output Log)
	UE_LOG(LogTemp, Warning, TEXT("GameMode StartPlay called"));
	UWorld* World = GetWorld();
	if (!World)
	{
		UE_LOG(LogTemp, Error, TEXT("StartPlay: GetWorld() returned null"));
		return;
	}

	const FString WorldName = World->GetName();
	AGameModeBase* AuthGM = World->GetAuthGameMode();
	UE_LOG(LogTemp, Warning, TEXT("World: %s, AuthGameMode: %s"), *WorldName, AuthGM ? *AuthGM->GetClass()->GetName() : TEXT("NULL"));
	UE_LOG(LogTemp, Warning, TEXT("GameMode DefaultPawnClass: %s"), DefaultPawnClass ? *DefaultPawnClass->GetName() : TEXT("NULL"));
	UE_LOG(LogTemp, Warning, TEXT("GameMode PlayerControllerClass: %s"), PlayerControllerClass ? *PlayerControllerClass->GetName() : TEXT("NULL"));

	// If a player already has a pawn, don't respawn
	APlayerController* PC = World->GetFirstPlayerController();
	if (!PC)
	{
		UE_LOG(LogTemp, Error, TEXT("StartPlay: No PlayerController returned by GetFirstPlayerController()"));
		return;
	}

	if (PC->GetPawn())
	{
		UE_LOG(LogTemp, Warning, TEXT("StartPlay: PlayerController already has pawn: %s"), *PC->GetPawn()->GetName());
		return;
	}

	// Find a PlayerStart or spawn at origin
	AActor* StartSpot = FindPlayerStart(PC);
	FTransform SpawnTransform = StartSpot ? StartSpot->GetActorTransform() : FTransform::Identity;

	if (!DefaultPawnClass)
	{
		UE_LOG(LogTemp, Error, TEXT("StartPlay: DefaultPawnClass is null, cannot spawn pawn"));
		return;
	}

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;

	APawn* NewPawn = World->SpawnActor<APawn>(DefaultPawnClass, SpawnTransform, SpawnParams);
	if (!NewPawn)
	{
		UE_LOG(LogTemp, Error, TEXT("StartPlay: SpawnActor failed for %s"), *DefaultPawnClass->GetName());
		return;
	}

	PC->Possess(NewPawn);
	UE_LOG(LogTemp, Warning, TEXT("StartPlay: Forced spawn and possess of pawn %s by controller %s"), *NewPawn->GetName(), *PC->GetName());
}

APawn* AUS_GameMode::SpawnDefaultPawnFor_Implementation(AController* NewPlayer, AActor* StartSpot)
{
	UE_LOG(LogTemp, Warning, TEXT("SpawnDefaultPawnFor called for controller: %s"),
		NewPlayer ? *NewPlayer->GetName() : TEXT("NULL"));

	// Do NOT forcibly override DefaultPawnClass to AUS_Character here!
	// Use the class chosen in constructor / StartPlay — do not forcibly override here.
	APawn* SpawnedPawn = Super::SpawnDefaultPawnFor_Implementation(NewPlayer, StartSpot);

	// Diagnostic: show spawned class name
	if (SpawnedPawn)
	{
		UE_LOG(LogTemp, Warning, TEXT("SpawnDefaultPawnFor spawned: %s (%s)"),
			*SpawnedPawn->GetName(), *SpawnedPawn->GetClass()->GetName());
	}

	return SpawnedPawn;
}