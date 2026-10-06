#pragma once

#include "CoreMinimal.h"
#include "VBAbilityContext.generated.h"

class AVBCharacter;

USTRUCT(BlueprintType) 
struct FAbilityContext 
{
	GENERATED_BODY();

	UPROPERTY(BlueprintReadOnly)
	AVBCharacter* Caster = nullptr;

	UPROPERTY(BlueprintReadOnly)
	FVector TargetLocation = FVector::ZeroVector;

	UPROPERTY(BlueprintReadOnly)
	AActor* TargetActor = nullptr;
};