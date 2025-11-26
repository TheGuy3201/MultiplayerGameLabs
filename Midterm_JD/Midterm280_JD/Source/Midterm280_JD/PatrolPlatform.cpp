// Fill out your copyright notice in the Description page of Project Settings.


#include "PatrolPlatform.h"

// Sets default values
APatrolPlatform::APatrolPlatform()
{
 	// Set this actor to call Tick() every frame.
	PrimaryActorTick.bCanEverTick = true;

	// Create the body component and set it as root
	Body = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Body"));
	SetRootComponent(Body);
}

// Called when the game starts or when spawned
void APatrolPlatform::BeginPlay()
{
	Super::BeginPlay();
	
	// Initialize start and target locations
	StartLocation = GetActorLocation();
	TargetLocation = StartLocation + MoveOffset;
}

// Called every frame
void APatrolPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdatePlatform(DeltaTime);
}

void APatrolPlatform::UpdatePlatform(float DeltaSeconds)
{
	// Determine destination based on direction
	FVector Dest = (Direction > 0) ? TargetLocation : StartLocation;
	
	// Get current location
	FVector Current = GetActorLocation();
	
	// Calculate travel direction
	FVector Dir = (Dest - Current).GetSafeNormal();
	
	// Calculate new location
	FVector NewLoc = Current + Dir * Speed * DeltaSeconds;
	
	// Check if we've reached the destination
	float DistToDest = FVector::Dist(Current, Dest);
	
	if (DistToDest <= EndTolerance)
	{
		// Snap to destination and flip direction
		NewLoc = Dest;
		Direction *= -1;
	}
	
	// Update actor location
	SetActorLocation(NewLoc);
}

