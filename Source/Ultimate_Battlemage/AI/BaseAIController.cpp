// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseAIController.h"
#include "Kismet/GameplayStatics.h"

ABaseAIController::ABaseAIController()
{
	Perception = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception"));
	SetPerceptionComponent(*Perception);

	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));

	SightConfig->SightRadius = 1200.f;
	SightConfig->LoseSightRadius = 1400.f; // usually slightly bigger
	SightConfig->PeripheralVisionAngleDegrees = 90.f;
	SightConfig->SetMaxAge(5.f);
	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;

	Perception->ConfigureSense(*SightConfig);
	Perception->SetDominantSense(UAISense_Sight::StaticClass());

	Perception->OnTargetPerceptionUpdated.AddDynamic(this, &ABaseAIController::OnTargetPerceptionUpdated);
}

void ABaseAIController::BeginPlay()
{
	Super::BeginPlay();
}

void ABaseAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
}

void ABaseAIController::OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	// Only react to sight (optional)
	if (Stimulus.Type != UAISense::GetSenseID(UAISense_Sight::StaticClass()))
		return;


	if (Stimulus.WasSuccessfullySensed())
	{
		CanSeePlayer = true;
		ChasePlayer(Actor);
	}
	else
	{
		CanSeePlayer = false;
		StopMovement();
	}
}

void ABaseAIController::ChasePlayer(AActor* Target)
{

	if (Target)
	{
		MoveToActor(Target, 500.f); // 100 = acceptance radius
	}
}



