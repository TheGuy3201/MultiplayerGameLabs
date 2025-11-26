// SpinnerBobber.h
#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpinnerBobber.generated.h"
UCLASS()
class MIDTERM280_JD_API ASpinnerBobber : public AActor
{
	GENERATED_BODY()
public:
	ASpinnerBobber();
	virtual void Tick(float DeltaSeconds) override;
protected:
	virtual void BeginPlay() override;
private:
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* Mesh;
	UPROPERTY(EditAnywhere, Category = "Spin")
	float YawDegreesPerSecond = 90.f;
	UPROPERTY(EditAnywhere, Category = "Bob")
	float BobAmplitude = 30.f;
	UPROPERTY(EditAnywhere, Category = "Bob")
	float BobFrequency = 1.f; // cycles per second
	float BaseZ = 0.f;
	float RunningTime = 0.f; // accumulate time for sine
	void UpdateSpin(float DeltaSeconds);
	void UpdateBob(float DeltaSeconds);
};