// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "US_Minion.generated.h"

// Forward declarations
class UPawnSensingComponent;
class USphereComponent;
class AUS_BasePickup;

UCLASS()
class US_LOTL_JD_API AUS_Minion : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AUS_Minion();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UFUNCTION()
	void OnHearNoise(APawn* PawnInstigator, const FVector& Location, float Volume);

	UFUNCTION()
	void OnDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Minion AI")
	float PatrolSpeed = 150.0f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Minion AI")
	float ChaseSpeed = 350.0f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Minion AI")
	float PatrolRadius = 50000.0f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Minion AI")
	float AlertRadius = 1000.0f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Minion Stats")
	float Health = 100.0f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Minion Loot")
	TSubclassOf<AUS_BasePickup> SpawnedPickup;

	UFUNCTION(BlueprintCallable, Category = "Minion AI")
	void SetNextPatrolLocation();
	
	UFUNCTION(BlueprintCallable, Category = "Minion AI")
	void Chase(APawn* Pawn);
	
	virtual void PostInitializeComponents() override;
	
	FORCEINLINE UPawnSensingComponent* GetPawnSense() const { return PawnSense; }
	FORCEINLINE USphereComponent* GetCollision() const { return Collision; }

	UFUNCTION()
	void OnPawnDetected(APawn* Pawn);
	
	UFUNCTION()
	void OnBeginOverlap(AActor* OverlappedActor, AActor* OtherActor);

	UFUNCTION(BlueprintCallable, Category = "Minion AI")
	void GoToLocation(const FVector& Location);

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Minion Perception", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UPawnSensingComponent> PawnSense;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Minion Perception", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USphereComponent> Collision;
	
	UPROPERTY()
	FVector PatrolLocation;
};
