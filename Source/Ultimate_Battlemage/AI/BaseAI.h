// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Ultimate_Battlemage/Stats/FStats.h"
#include "Ultimate_Battlemage/Spells/ProjectileSpell.h"
//#include "Perception/AIPerceptionComponent.h"
//#include "Perception/AISenseConfig_Sight.h"
#include "BaseAI.generated.h"

//class UAIPerceptionComponent;
//class UAISenseConfig_Sight;

UCLASS()
class ULTIMATE_BATTLEMAGE_API ABaseAI : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseAI();

	//AI Functions
	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	

	//AI Components
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float CurrHealth;

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
	FStatsStruct AIStats;

	//Widget Variables
	UFUNCTION(BlueprintPure, Category = "Health")
	float GetHealthPercentage() const { return CurrHealth / MaxHealth; }

	//Widget Functions
	UFUNCTION()
	void UpdateHealthBarVisuals();

	//AI Variables
	
	//AI Spell
	UPROPERTY(EditDefaultsOnly, Category = "Spell")
	TSubclassOf<AProjectileSpell> MySpell;
};
