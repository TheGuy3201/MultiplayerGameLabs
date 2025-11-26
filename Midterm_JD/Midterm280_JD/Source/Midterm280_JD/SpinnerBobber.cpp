// Fill out your copyright notice in the Description page of Project Settings.


#include "SpinnerBobber.h"

// Sets default values
ASpinnerBobber::ASpinnerBobber()
{
 	// Set this actor to call Tick() every frame.
	PrimaryActorTick.bCanEverTick = true;

	// Create the mesh component and set it as root
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(Mesh);
}

// Called when the game starts or when spawned
void ASpinnerBobber::BeginPlay()
{
	Super::BeginPlay();
	
	// Store the initial Z position as the base for bobbing
	BaseZ = GetActorLocation().Z;
}

// Called every frame
void ASpinnerBobber::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Accumulate time for sine wave calculation
	RunningTime += DeltaTime;

	// Update spinning and bobbing
	UpdateSpin(DeltaTime);
	UpdateBob(DeltaTime);
}

void ASpinnerBobber::UpdateSpin(float DeltaSeconds)
{
	// Add yaw rotation
	AddActorLocalRotation(FRotator(0.f, YawDegreesPerSecond * DeltaSeconds, 0.f));
}

void ASpinnerBobber::UpdateBob(float DeltaSeconds)
{
	// Calculate new Z position using sine wave
	float NewZ = BaseZ + BobAmplitude * FMath::Sin(2 * PI * BobFrequency * RunningTime);
	
	// Get current location and update only the Z component
	FVector CurrentLocation = GetActorLocation();
	CurrentLocation.Z = NewZ;
	
	// Set the new location
	SetActorLocation(CurrentLocation);
}

