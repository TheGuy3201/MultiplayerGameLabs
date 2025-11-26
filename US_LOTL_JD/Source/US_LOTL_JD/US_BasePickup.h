// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "US_BasePickup.generated.h"

class USphereComponent;
class UStaticMeshComponent;
class AUS_Character;

UCLASS()
class US_LOTL_JD_API AUS_BasePickup : public AActor
{
	GENERATED_BODY()

public:
	AUS_BasePickup();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	// Sphere collision for pickup detection
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pickup")
	USphereComponent* SphereCollision;

	// Mesh for visual representation (always present, set mesh in Blueprint defaults)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pickup")
	UStaticMeshComponent* Mesh;

	// Called when overlap begins
	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:
	// BlueprintNativeEvent for pickup logic, can be overridden in BP
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Pickup")
	void Pickup(AUS_Character* OwningCharacter);
	virtual void Pickup_Implementation(AUS_Character* OwningCharacter);
};
