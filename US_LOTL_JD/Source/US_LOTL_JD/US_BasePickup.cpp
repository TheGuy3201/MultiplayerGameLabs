// Fill out your copyright notice in the Description page of Project Settings.

#include "US_BasePickup.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "US_Character.h"

// Sets default values
AUS_BasePickup::AUS_BasePickup()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	RootComponent = SphereCollision;
	SphereCollision->SetGenerateOverlapEvents(true);
	SphereCollision->SetSphereRadius(200.f);
	SphereCollision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(SphereCollision);
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	bReplicates = true; //Actors have this false by default
}

// Called when the game starts or when spawned
void AUS_BasePickup::BeginPlay()
{
	Super::BeginPlay();
	if (SphereCollision)
	{
		SphereCollision->OnComponentBeginOverlap.AddDynamic(this, &AUS_BasePickup::OnBeginOverlap);
	}
	// Warn if mesh is not set
	if (!Mesh || !Mesh->GetStaticMesh())
	{
		UE_LOG(LogTemp, Warning, TEXT("BasePickup: Mesh component is missing or has no StaticMesh set. Set mesh in Blueprint defaults."));
	}
}

void AUS_BasePickup::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AUS_Character* Character = Cast<AUS_Character>(OtherActor);
	if (Character)
	{
		Pickup(Character);
	}
}

void AUS_BasePickup::Pickup_Implementation(AUS_Character* OwningCharacter)
{
	if (OwningCharacter)
	{
		SetOwner(OwningCharacter);
		// Add additional pickup logic here (e.g., destroy, give item, etc.)
	}
}

// Called every frame
void AUS_BasePickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


