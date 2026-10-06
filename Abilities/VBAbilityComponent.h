#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "VBAbilityTypes.h"
#include "VBAbilityLoadout.h"
#include "VBAbilityComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class VEILBOUND_API UVBAbilityComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	UVBAbilityComponent();

	FAbilityLoadout* GetActiveDomainLoadout();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities")
	EAbilityDomain ActiveDomain = EAbilityDomain::Light;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities")
	TMap<EAbilityDomain, FAbilityLoadout> DomainLoadouts;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities")
	FAbilityLoadout HotKeyLayout;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities")
	TArray<TObjectPtr<UVBAbility>> CoreAbilities;

	UFUNCTION(BlueprintCallable, Category = "Abilities")
	void InitializeDomainLoadouts();


};