// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "US_PlayerState.generated.h"

/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnXpChanged, int32, NewXp);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCharacterLevelUp, int32, NewLevelXp);

UCLASS()
class US_LOTL_JD_API AUS_PlayerState : public APlayerState
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Category = "Experience")
	void AddXp(int32 Value);
	
	UFUNCTION(BlueprintPure, Category = "Experience")
	int32 GetXp() const { return Xp; }
	
	UFUNCTION(BlueprintPure, Category = "Experience")
	int32 GetCharacterLevel() const { return CharacterLevel; }
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, ReplicatedUsing = "OnRep_Xp", Category = "Experience")
	int32 Xp = 0;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, ReplicatedUsing = "OnRep_CharacterLevelUp", Category = "Experience")
	int32 CharacterLevel = 1;
	
	UFUNCTION()
	void OnRep_Xp(int32 OldValue) const;
	
	UFUNCTION()
	void OnRep_CharacterLevelUp(int32 OldValue) const;

public:
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnXpChanged OnXpChanged;
	
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnCharacterLevelUp OnCharacterLevelUp;
};
