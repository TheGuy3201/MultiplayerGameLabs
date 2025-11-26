// PatrolPlatform.h
#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PatrolPlatform.generated.h"
UCLASS()
class MIDTERM280_JD_API APatrolPlatform : public AActor
{
	GENERATED_BODY()
public:
	APatrolPlatform();
	virtual void Tick(float DeltaSeconds) override;
protected:
	virtual void BeginPlay() override;
private:
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* Body;
	UPROPERTY(EditAnywhere, Category = "Movement")
	FVector MoveOffset = FVector(600.f, 0.f, 0.f);
	UPROPERTY(EditAnywhere, Category = "Movement", meta = (ClampMin = "0.0"))
	float Speed = 200.f;
	UPROPERTY(EditAnywhere, Category = "Movement", meta = (ClampMin = "1.0"))
	float EndTolerance = 5.f;
	FVector StartLocation;
	FVector TargetLocation;
	int32 Direction = +1; // +1 towards Target, -1 back to Start
	// TODO: helper to check end reached and flip direction
	void UpdatePlatform(float DeltaSeconds);
};