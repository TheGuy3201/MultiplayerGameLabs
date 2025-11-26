// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "US_WeaponProjectileComponent.generated.h"

// Forward declaration
class AS_BaseWeaponProjectile;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class US_LOTL_JD_API UUS_WeaponProjectileComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UUS_WeaponProjectileComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
	void Throw();
	
	UFUNCTION(Server, Reliable)
	void Throw_Server();

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	UFUNCTION(BlueprintCallable, Category = "Projectile")
	void SetProjectileClass(TSubclassOf<AS_BaseWeaponProjectile> NewProjectileClass);

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AS_BaseWeaponProjectile> ProjectileClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* WeaponMappingContext;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	class UInputAction* ThrowAction;
};
