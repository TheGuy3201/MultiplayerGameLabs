// Fill out your copyright notice in the Description page of Project Settings.

#include "US_PlayerState.h"
#include "US_Character.h"
#include "US_CharacterStats.h"
#include "Net/UnrealNetwork.h"
#include "Engine/Engine.h"

void AUS_PlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AUS_PlayerState, Xp);
	DOREPLIFETIME(AUS_PlayerState, CharacterLevel);
}

void AUS_PlayerState::AddXp(const int32 Value)
{
	Xp += Value;
	OnXpChanged.Broadcast(Xp);
	
	if (const auto Character = Cast<AUS_Character>(GetPawn()))
	{
		const auto CharacterStats = Character->GetCharacterStats();
		if (CharacterStats && Xp >= CharacterStats->NextLevelXp)
		{
			CharacterLevel++;
			Character->UpdateCharacterStats(CharacterLevel);
			OnCharacterLevelUp.Broadcast(CharacterLevel);
		}
	}
}

void AUS_PlayerState::OnRep_Xp(int32 OldValue) const
{
	OnXpChanged.Broadcast(Xp);
}

void AUS_PlayerState::OnRep_CharacterLevelUp(int32 OldValue) const
{
	OnCharacterLevelUp.Broadcast(CharacterLevel);
}