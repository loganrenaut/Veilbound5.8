#pragma once

#include "CoreMinimal.h"
#include "VBAbilityTypes.generated.h"

UENUM(BlueprintType)
enum class EAbilityDomain : uint8
{
	Light,
	Dark,
	Space,
	Time,
	Elemental,
	Etheral,
	Mental,

	MAX UMETA(Hidden)
};