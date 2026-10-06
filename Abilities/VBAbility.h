#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "VBAbilityTypes.h"
#include "VBAbilityContext.h"
#include "VBAbilityCastingTypes.h"
#include "VBAbility.generated.h"

UCLASS(Blueprintable)
class VEILBOUND_API UVBAbility : public UObject
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability")
	FName AbilityName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability")
	EAbilityDomain Domain;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability|Cost")
	float ResourceCost = 0.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability")
	FAbilityCastingPattern CastingPattern;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Ability")
	void Execute(const FAbilityContext& Context);
};