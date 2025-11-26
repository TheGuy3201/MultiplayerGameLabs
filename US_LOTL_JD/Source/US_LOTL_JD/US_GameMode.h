// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "US_GameMode.generated.h"

/**
 * 
 */
UCLASS()
class US_LOTL_JD_API AUS_GameMode : public AGameMode
{
	GENERATED_BODY()

public:
	AUS_GameMode();

protected:
	virtual void StartPlay() override;
	virtual APawn* SpawnDefaultPawnFor_Implementation(AController* NewPlayer, AActor* StartSpot) override;

public:
	UFUNCTION(BlueprintCallable, Category = "Minions")
	void AlertMinions(AActor* AlertInstigator, float Radius, FVector Location);
};
