#pragma once

#include "CoreMinimal.h"
#include "VBAbilityCastingTypes.generated.h"


USTRUCT(BlueprintType)
struct FAbilityInput
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Casting")
	FKey key;
};

UENUM(BlueprintType)
enum class EAbilityActivationMode : uint8
{
	Press,
	Hold, 
	DoublePress,

	Max UMETA(Hidden)
};

UENUM(BlueprintType)
enum class EAbilityCastingState : uint8
{
	Idle,
	Casting,
	Interrupted,
	
	Max UMETA(Hidden)
};

USTRUCT(BlueprintType)
struct FAbilityCastingPattern
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Casting")
	TArray<FAbilityInput> Inputs;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Casting")
	EAbilityActivationMode ActivationMode = EAbilityActivationMode::Press;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Casting")
	float MaxPressInterval = 0.3f;

};