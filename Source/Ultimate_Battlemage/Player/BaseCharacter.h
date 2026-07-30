// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Ultimate_Battlemage/Stats/FStats.h"
#include "Ultimate_Battlemage/Spells/BaseSpell.h"
#include "Ultimate_Battlemage/Spells/SpellCastContext.h"
#include "Ultimate_Battlemage/Spells/ProjectileSpell.h"
#include "Ultimate_Battlemage/Spells/TraceSpell.h"
#include "Ultimate_Battlemage/Spells/AOESpell.h"
#include "InputActionValue.h"
#include "Engine/World.h"
#include "EngineMinimal.h"
#include "BaseCharacter.generated.h"

// Forward declarations (prevents circular includes)
//class UInputMappingContext;
class UInputAction;
class UCameraComponent;
class USpringArmComponent;
//class ABasePlayerController;
//class FStats;

UCLASS()
class ULTIMATE_BATTLEMAGE_API ABaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties --- Constructor ---
	ABaseCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// --- Movement Actions ---
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	

	// --- Sprint Actions ---
	void StartSprint();
	void StopSprint();

	// --- Cast Actoins ---
	void CastPrimary();
	void AOEUpdateCast();
	void AOEStartCast();
	void AOEReleaseCast();
	void ProjCast();
	void TraceCast();

	// --- AOE Spell Adjust ---
	void IncreaseAOERange();
	void DecreaseAOERange();

	// --- Mana ---
	void RegenMana(float Dt);

	// --- Stamina ---
	void DrainStamina();
	void RegenStamina();

	// --- Helpers ---
	void UpdateHud(float Var, float MaxVar);

	// --- Jump is handled by built in ACharacter Jump(), but can override if needed ---
public:

	// ---Scripts---
	/*UPROPERTY(BlueprintReadOnly, Category = "Scripts")
	ABasePlayerController* PC;*/




	// --- Stats (Base placeholders) ---
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float Health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float MaxHealth;

	// --- Resource Stats ---
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float Mana;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float MaxMana;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float ManaRegen; //Per Second

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float Stamina;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float MaxStamina;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float StaminaRegin; //Per Second

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float SprintStaminaDrain; //Per Second




	// --- Stats Struct Instance ---
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	FStatsStruct CharacterStats;




	// --- Sprint settings ---
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float WalkSpeed = 400.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float SprintSpeed = 800.f;

	bool bIsSprintg = false;




	// --- Components ---
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	USpringArmComponent* CameraBoom;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	UCameraComponent* FollowCamera;




	// --- Spells ---
	UPROPERTY(EditDefaultsOnly, Category = "Spell")
	TSubclassOf<ABaseSpell> PrimarySpellClass;

	UPROPERTY(EditDefaultsOnly, Category = "Spell")
	TSubclassOf<AProjectileSpell> ProjSpellClass;

	UPROPERTY(EditDefaultsOnly, Category = "Spell")
	TSubclassOf<ATraceSpell> TraceSpellClass;

	UPROPERTY(EditDefaultsOnly, Category = "Spell")
	TSubclassOf<AAOESpell> AOESpellClass;

	/*UPROPERTY(EditDefaultsOnly, Category = "Spell")
	TSubclassOf<AAOESpell> AOESpellClass;*/

	UPROPERTY(EditDefaultsOnly, Category = "Spell")
	float PrimarySpellManaCost = 10.f;

	UPROPERTY(EditDefaultsOnly, Category = "Spell")
	float PrimarySpellCooldown = 1.0f;

	bool bCanCast = true;

	FSpellCastContext Context;




	// --- AOE/Hold Spells ---
	bool bIsTargeting = false;

	UPROPERTY()
	AActor* AOEDecal;

	UPROPERTY()
	FVector TargetPoint;

	UPROPERTY()
	float AOERange = 1000.f;

	UPROPERTY()
	FVector end;




	

	// --- Input System ---
	/*UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputMappingContext* InputMappingContext;*/

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* MoveAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* LookAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* JumpAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* SprintAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* CastAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* AOEAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* ProjAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* TraceAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* AOEExtend;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* AOERetract;

	UPROPERTY(EditAnywhere)
	ABaseSpell* Spell;

	FActorSpawnParameters SpawnParams;


	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input --- Input Setup ---
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Called when taking damage
	//void TakeDamage(float amount);

	//// Called when using mana
	//void UseMana(float amount);
};
