// Fill out your copyright notice in the Description page of Project Settings.

#include "VBCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "TimerManager.h"
#include "Components/CapsuleComponent.h"

// Sets default values
AVBCharacter::AVBCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	AbilityComponent = CreateDefaultSubobject<UVBAbilityComponent>(TEXT("AbilityComponent"));

}

// Called when the game starts or when spawned
void AVBCharacter::BeginPlay()
{
	Super::BeginPlay();

	CurrentHealth = MaxHealth;

	UE_LOG(LogTemp, Warning, TEXT("Health: %f/%f"), CurrentHealth, MaxHealth);
	UE_LOG(LogTemp, Warning, TEXT("Stamina: %f/%f"), CurrentStamina, MaxStamina);

	GetWorldTimerManager().SetTimer(HealthRegenTimerHandle, this, &AVBCharacter::RegenerateHealth, 1.0f, true);
	GetWorldTimerManager().SetTimer(StaminaRegenTimerHandle, this, &AVBCharacter::RegenerateStamina, 1.0f, true);
	
}

// Apply damage to the character
void AVBCharacter::ApplyDamage(float DamageAmount)
{
	if (bIsDead || DamageAmount <= 0.0f) {
		return;
	}

	CurrentHealth = FMath::Max(0.0f, CurrentHealth - DamageAmount);
	UE_LOG(LogTemp, Warning, TEXT("Health: %f/%f"), CurrentHealth, MaxHealth);

	if (CurrentHealth <= 0.0f) {
		bIsDead = true;
		bIsFainted = false;
		bIsInCombat = false;
		GetCharacterMovement()->DisableMovement();
		UE_LOG(LogTemp, Warning, TEXT("%s has died."), *CharacterDisplayName);
		OnDeath();
	}

	else if (CurrentHealth <= 5.0f && !bIsFainted) {
		bIsFainted = true;
		bIsPerformingStaminaAction = false;
		bIsSprinting = false;
		CurrentStamina = 0.0f;
		GetCharacterMovement()->DisableMovement();
		UE_LOG(LogTemp, Warning, TEXT("%s has fainted."), *CharacterDisplayName);
		OnFaint();
	}
}

// Apply healing to the character
void AVBCharacter::ApplyHealing(float HealAmount)
{
	if (bIsDead || HealAmount <= 0.0f) {
		return;
	}
	CurrentHealth = FMath::Min(MaxHealth, CurrentHealth + HealAmount);
	UE_LOG(LogTemp, Warning, TEXT("Health: %f/%f"), CurrentHealth, MaxHealth);

	if (bIsFainted && CurrentHealth > 5.0f) {
		bIsFainted = false;
		GetCharacterMovement()->SetMovementMode(MOVE_Walking);
		UE_LOG(LogTemp, Warning, TEXT("%s has recovered from fainting."), *CharacterDisplayName);
	}
}

// Revive the character if they are fainted
void AVBCharacter::ReviveCharacter()
{
	if (bIsDead || !bIsFainted) {
		return;
	}
	
	CurrentHealth = FMath::Min(MaxHealth, CurrentHealth + 25.0f);
	UE_LOG(LogTemp, Warning, TEXT("Health: %f/%f"), CurrentHealth, MaxHealth);
	bIsFainted = false;
	GetCharacterMovement()->SetMovementMode(MOVE_Walking);
	UE_LOG(LogTemp, Warning, TEXT("%s has been revived."), *CharacterDisplayName);
}

// Revive a teammate if they are fainted
void AVBCharacter::ReviveTeammate(AVBCharacter* TargetCharacter)
{
	if (TargetCharacter == this || TargetCharacter == nullptr) {
		UE_LOG(LogTemp, Warning, TEXT("%s cannot revive themselves."), *CharacterDisplayName);
		return;
	}

	if (!TargetCharacter || TargetCharacter->bIsDead || !TargetCharacter->bIsFainted) {
		return;
	}
	
	const float ReviveRange = 200.0f; // Define a range within which the teammate can be revived

	if (FVector::DistSquared(GetActorLocation(), TargetCharacter->GetActorLocation())
	> FMath::Square(ReviveRange)) {
		UE_LOG(LogTemp, Warning, TEXT("%s is too far away to revive %s."), *CharacterDisplayName, *TargetCharacter->CharacterDisplayName);
		return;
	}
	TargetCharacter->ReviveCharacter();
	UE_LOG(LogTemp, Warning, TEXT("%s has revived %s."), *CharacterDisplayName, *TargetCharacter->CharacterDisplayName);
}

void AVBCharacter::ActivateCombatMode()
{
	if (bIsDead) {
		return;
	}
	bIsInCombat = true;
	UE_LOG(LogTemp, Warning, TEXT("%s has entered combat mode."), *CharacterDisplayName);
}

void AVBCharacter::DeactivateCombatMode()
{
	if (bIsDead) {
		return;
	}
	bIsInCombat = false;
	UE_LOG(LogTemp, Warning, TEXT("%s has exited combat mode."), *CharacterDisplayName);
}

void AVBCharacter::RegenerateHealth()
{
	if (bIsDead || bIsFainted) {
		return;
	}
	
	if (CurrentHealth >= MaxHealth) {
		return;
	}

	float RegenRate = bIsInCombat ? 1.0f : 5.0f; // Health per second

	CurrentHealth = FMath::Min(MaxHealth, CurrentHealth + RegenRate);

	UE_LOG(LogTemp, Warning, TEXT("Health: %f/%f"), CurrentHealth, MaxHealth);
}

void AVBCharacter::RegenerateStamina() {

	if (bIsDead) {
		return;
	}

	if (bIsSprinting) {
		CurrentStamina = FMath::Max(0.0f, CurrentStamina - SprintStaminaCost);


		if (CurrentStamina <= 0.0f) {
			StopSprinting();
		}

		return;
	}

	if (CurrentStamina >= MaxStamina) {
		return;
	}

	float RegenRate = bIsInCombat ? CombatStaminaRegen : OutOfCombatStaminaRegen;

	CurrentStamina = FMath::Min(MaxStamina, CurrentStamina + RegenRate);

	UE_LOG(LogTemp, Warning, TEXT("Stamina: %f/%f"), CurrentStamina, MaxStamina);

	//Enough stamina will awaken the player from being fainted
	if (CurrentStamina >= 20.0f && bIsFainted) {
		bIsFainted = false;
		GetCharacterMovement()->SetMovementMode(MOVE_Walking);

		UE_LOG(LogTemp, Warning, TEXT("%s has regained consciousness"), *CharacterDisplayName);
	}
}

// Called every frame
void AVBCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AVBCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AVBCharacter::StartSprinting() {
	if (bIsDead || bIsFainted || CurrentStamina <= 0.0f) {
		return;
	}

	if (bIsSprinting) {
		return;
	}

	bIsSprinting = true;
	bIsPerformingStaminaAction = true;

	GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
}

void AVBCharacter::StopSprinting() {
	if (!bIsSprinting) {
		return;
	}

	bIsSprinting = false;
	bIsPerformingStaminaAction = false;

	GetCharacterMovement()->MaxWalkSpeed = 300.0f;
}

void AVBCharacter::StartDodging() {
	if (bIsDead || bIsFainted || bIsDodging || bIsRecoveringFromDodge) {
		return;
	}

	if (CurrentStamina <= DodgeStaminaCost) {
		return;
	}

	if (bIsSprinting) {
		StopSprinting();
	}

	CurrentStamina -= DodgeStaminaCost;

	bIsDodging = true;
	bIsPerformingStaminaAction = true;

	FVector DodgeDirection = GetLastMovementInputVector().GetSafeNormal2D();

	if (DodgeDirection.IsNearlyZero()) {
		DodgeDirection = GetActorForwardVector();
	}

	FVector DodgeVelocity = DodgeDirection * (DodgeDistance / DodgeDuration);

	LaunchCharacter(DodgeVelocity, true, false);

	GetWorldTimerManager().SetTimer(
		DodgeTimerHandle, 
		this, 
		&AVBCharacter::StopDodging, 
		DodgeDuration, 
		false);
}

void AVBCharacter::StopDodging() {
	if (!bIsDodging) {
		return;
	}

	GetCharacterMovement()->Velocity.X = 0.0f;
	GetCharacterMovement()->Velocity.Y = 0.0f;

	bIsDodging = false;
	bIsPerformingStaminaAction = true;

	GetWorldTimerManager().SetTimer(
		DodgeRecoveryTimerHandle,
		this,
		&AVBCharacter::FinishDodgeRecovery,
		DodgeRecoveryDuration,
		false
	);
}

void AVBCharacter::FinishDodgeRecovery() {
	bIsRecoveringFromDodge = false;
	bIsPerformingStaminaAction = false;
}

void AVBCharacter::TeleportCharacter(FVector Direction, float Distance) {

	Direction.Z = 0.0f;

	Direction = Direction.GetSafeNormal();

	FVector StartLocation = GetActorLocation();
	FVector DesiredLocation = StartLocation + (Direction * Distance);

	FHitResult HitResult;

	bool bBlocked = GetWorld()->SweepSingleByChannel(
		HitResult,
		StartLocation,
		DesiredLocation,
		FQuat::Identity,
		ECC_Visibility,
		FCollisionShape::MakeCapsule(
			GetCapsuleComponent()->GetScaledCapsuleRadius(),
			GetCapsuleComponent()->GetScaledCapsuleHalfHeight()
		)
	);

	if (bBlocked) {
		FVector SafeLocation = HitResult.Location - (Direction * 10.0f);
		SetActorLocation(SafeLocation);
	}
	else {
		SetActorLocation(DesiredLocation);
	}
}

void AVBCharacter::TeleportCharacterToTarget(FVector TargetLocation, float Distance) {

	FVector Destination = TargetLocation;
	float CapsuleHeight = GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
	Destination.Z += CapsuleHeight;

	FCollisionShape CapsuleShape = FCollisionShape::MakeCapsule(
		GetCapsuleComponent()->GetScaledCapsuleRadius(),
		CapsuleHeight
	);

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);

	FHitResult HitResult;

	bool bBlocked = GetWorld()->SweepSingleByChannel(
		HitResult,
		Destination,
		Destination,
		FQuat::Identity,
		ECC_Visibility,
		CapsuleShape
	);

	//if (bBlocked) {
	//	return;
	//}

	SetActorLocation(Destination);
}

FTargetingResult AVBCharacter::GetTargetLocation(float MaxRange) {
	FTargetingResult Result;
	FVector CameraLocation;
	FRotator CameraRotation;

	GetActorEyesViewPoint(CameraLocation, CameraRotation);

	FVector TraceStart = CameraLocation;
	FVector TraceEnd = TraceStart + (CameraRotation.Vector() * 10000.0f);

	FHitResult HitResult;

	bool bHit = GetWorld()->LineTraceSingleByChannel(
		HitResult,
		TraceStart,
		TraceEnd,
		ECC_Visibility
	);

	FVector TargetLocation = bHit ? HitResult.Location : TraceEnd;

	//Keep targeting horizontal for range calculation.
	FVector StartLocation = GetActorLocation();

	FVector Start2D = FVector(StartLocation.X, StartLocation.Y, 0.0f);
	FVector Target2D = FVector(TargetLocation.X, TargetLocation.Y, 0.0f);

	FVector Direction = Target2D - Start2D;
	float DistanceToTarget = Direction.Size();

	Direction = Direction.GetSafeNormal();

	//Clamp the target to the maximum horizontal range.
	if (DistanceToTarget > MaxRange) {
		FVector ClampedTarget = Start2D + (Direction * MaxRange);

		TargetLocation.X = ClampedTarget.X;
		TargetLocation.Y = ClampedTarget.Y;
	}

	FVector GroundTraceStart = TargetLocation;
	GroundTraceStart.Z += 5000.0f;

	FVector GroundTraceEnd = TargetLocation;
	GroundTraceEnd.Z -= 5000.0f;

	FHitResult GroundHit;

	bool bFoundGround = GetWorld()->LineTraceSingleByChannel(
		GroundHit,
		GroundTraceStart,
		GroundTraceEnd,
		ECC_Visibility
	);

	if (bFoundGround) {
		TargetLocation.Z = GroundHit.Location.Z;
		Result.bIsValid = true;
	}

	Result.TargetLocation = TargetLocation;
	return Result;

}