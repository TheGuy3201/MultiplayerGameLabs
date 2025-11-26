#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "US_Interactable.generated.h"

// Forward declaration
class AUS_Character;

UINTERFACE(MinimalAPI, Blueprintable)
class UUS_Interactable : public UInterface
{
	GENERATED_BODY()
};

class US_LOTL_JD_API IUS_Interactable
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interaction", meta = (DisplayName = "Interact"))
	void Interact(AUS_Character* CharacterInstigator);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interaction", meta = (DisplayName = "Can Interact"))
	bool CanInteract(AUS_Character* CharacterInstigator) const;
};