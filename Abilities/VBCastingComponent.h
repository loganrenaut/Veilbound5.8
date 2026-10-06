// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "VBAbilityCastingTypes.h"
#include "VBCastingComponent.generated.h"

class UVBAbility;


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class VEILBOUND_API UVBCastingComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UVBCastingComponent();

	UPROPERTY(BlueprintReadOnly, Category = "Casting")
	EAbilityCastingState CastingState = EAbilityCastingState::Idle;

	UPROPERTY(BlueprintReadOnly, Category = "Casting")
	TObjectPtr<UVBAbility> ActiveAbility = nullptr;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
