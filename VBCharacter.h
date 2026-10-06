// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Abilities/VBAbilityComponent.h"
#include "VBCharacter.generated.h"

USTRUCT(BlueprintType)
struct FTargetingResult {

	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	FVector TargetLocation = FVector::ZeroVector;

	UPROPERTY(BlueprintReadOnly)
	bool bIsValid = false;
};

UCLASS()
class VEILBOUND_API AVBCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Abilities")
	TObjectPtr<UVBAbilityComponent> AbilityComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
	FString CharacterDisplayName = TEXT("Veilbound Character");

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character|Health")
	float MaxHealth = 100.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character|Health")
	float CurrentHealth = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character|Stamina")
	float MaxStamina = 200.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character|Stamina")
	float CurrentStamina = 200.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character|Stamina")
	float OutOfCombatStaminaRegen = 5.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character|Stamina")
	float CombatStaminaRegen = 8.0f;

	UFUNCTION(BlueprintCallable, Category = "Character|Health")
	void ApplyDamage(float DamageAmount);

	UFUNCTION(BlueprintCallable, Category = "Character|Health")
	void ApplyHealing(float HealAmount);

	UFUNCTION(BlueprintCallable, Category = "Character|Health")
	void ReviveCharacter();

	UFUNCTION(BlueprintCallable, Category = "Character|Health")
	void ReviveTeammate(AVBCharacter* TargetCharacter);

	UFUNCTION(BlueprintImplementableEvent, Category = "Character|Health")
	void OnFaint();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character|Health")
	bool bIsFainted = false;

	UFUNCTION(BlueprintImplementableEvent, Category = "Character|Health")
	void OnDeath();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character|Health")
	bool bIsDead = false;

	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	bool bIsInCombat = false;

	UPROPERTY(BlueprintReadOnly, category = "Stamina")
	bool bIsPerformingStaminaAction = false;

	UPROPERTY(BlueprintReadOnly, Category = "Stamina")
	bool bIsSprinting = false;

	UPROPERTY(BlueprintReadOnly, Category = "Stamina")
	bool bIsDodging = false;

	UPROPERTY(BlueprintReadOnly, Category = "Dodge")
	bool bIsRecoveringFromDodge = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dodge")
	float DodgeStaminaCost = 25.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dodge")
	float DodgeDuration = 0.4f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dodge")
	float DodgeRecoveryDuration = 0.6f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dodge")
	float DodgeDistance = 400.0f;

	UFUNCTION(BlueprintCallable, Category = "Movement")
	void TeleportCharacter(FVector Direction, float Distance);

	UFUNCTION(BlueprintCallable, Category = "Movement")
	void TeleportCharacterToTarget(FVector TargetLocation, float MaxDistance);

	UFUNCTION(BlueprintCallable, Category = "Targeting")
	FTargetingResult GetTargetLocation(float MaxRange);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character|Stamina")
	float SprintSpeed = 600.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character|Stamina")
	float SprintStaminaCost = 5.0f;

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void ActivateCombatMode();

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void DeactivateCombatMode();

	UFUNCTION(BlueprintCallable, Category = "Stamina")
	void StartSprinting();

	UFUNCTION(BlueprintCallable, Category = "Stamina")
	void StopSprinting();

	UFUNCTION(BlueprintCallable, Category = "Dodge")
	void StartDodging();

	UFUNCTION(BlueprintCallable, Category = "Dodge")
	void StopDodging();

	void FinishDodgeRecovery();

	void RegenerateHealth();

	void RegenerateStamina();


	// Sets default values for this character's properties
	AVBCharacter();

	FTimerHandle HealthRegenTimerHandle;
	FTimerHandle StaminaRegenTimerHandle;
	FTimerHandle DodgeTimerHandle;
	FTimerHandle DodgeRecoveryTimerHandle;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
