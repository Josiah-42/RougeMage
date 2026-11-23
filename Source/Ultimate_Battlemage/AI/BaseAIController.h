// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
//Spells
#include "Ultimate_Battlemage/Spells/BaseSpell.h"
#include "Ultimate_Battlemage/Spells/SpellCastContext.h"
#include "Ultimate_Battlemage/Spells/ProjectileSpell.h"
#include "Ultimate_Battlemage/Spells/TraceSpell.h"
#include "Ultimate_Battlemage/Spells/AOESpell.h"
//
#include "BaseAIController.generated.h"

class UAIPerceptionComponent;
class UAISenseConfig_Sight;

/**
 * 
 */
UCLASS()
class ULTIMATE_BATTLEMAGE_API ABaseAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	ABaseAIController();

protected:
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;
	virtual void Tick(float DeltaTime) override;

public:

	//AI Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI Perception")
	UAIPerceptionComponent* Perception;

	UPROPERTY()
	UAISenseConfig_Sight* SightConfig;

	//AI Variables
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI Variables")
	bool CanSeePlayer;

	UPROPERTY(BlueprintReadOnly, Category = "AI Variables")
	AActor* TargetPlayer;

	UPROPERTY(BlueprintReadOnly, Category = "AI Variables")
	AActor* Me;

	UPROPERTY(BlueprintReadOnly, Category = "AI Variables")
	ABaseAI* myActor;

	//AI Spell Variables
	UPROPERTY(EditAnywhere)
	ABaseSpell* Spell1;

	FSpellCastContext Context;

		// -- Spells
		/*UPROPERTY(EditDefaultsOnly, Category = "Spell")
		TSubclassOf<AProjectileSpell> ProjSpellClass;*/

	//Functions

	UFUNCTION() 
	void OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);

	UFUNCTION()
	void ChasePlayer(AActor* Target);

	UFUNCTION()
	void AttackPlayer(AActor* Target, TSubclassOf<ABaseSpell> spell);

	UFUNCTION()
	void UpdateAim(float DeltaTime);


	// --- Spell Lock ---
	bool bIsCasting = false;
	FTimerHandle SpellDelay;

	void StartSpellLockout(float Duration);
	void ResetSpellCastLockout();
};
