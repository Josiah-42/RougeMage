// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
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

	//Functions
	void ChasePlayer(AActor* Target);


	UFUNCTION() 
	void OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);
};
