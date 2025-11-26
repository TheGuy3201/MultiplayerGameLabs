// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "US_PlayerController.generated.h"

/**
 * 
 */
UCLASS()
class US_LOTL_JD_API AUS_PlayerController : public APlayerController
{
	GENERATED_BODY()

public:
    AUS_PlayerController();

protected:
    virtual void BeginPlay() override;
    virtual void OnPossess(APawn* InPawn) override;
};
