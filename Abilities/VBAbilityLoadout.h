#pragma once

#include "CoreMinimal.h"
#include "VBAbilityLoadout.generated.h"	

class UVBAbility;

USTRUCT(BlueprintType)
struct FAbilityLoadout
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities")
	TArray<TObjectPtr<UVBAbility>> Abilities;
};
