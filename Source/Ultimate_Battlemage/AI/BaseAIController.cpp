// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseAIController.h"
#include "Kismet/GameplayStatics.h"
#include "BaseAI.h"

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

	// Initialize pointers
	TargetPlayer = nullptr;
	CanSeePlayer = false;
}

void ABaseAIController::BeginPlay()
{
	Super::BeginPlay();
}

void ABaseAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
}

// 2. IMPLEMENT TICK TO CONSTANTLY REFRESH MOVEMENT
void ABaseAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CanSeePlayer && TargetPlayer)
	{
		ChasePlayer(TargetPlayer);
	}
}


void ABaseAIController::OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	ABaseAI* myActor = Cast<ABaseAI>(Me);

	// Only react to sight (optional)
	if (Stimulus.Type != UAISense::GetSenseID(UAISense_Sight::StaticClass()))
		return;


	//Ignore Self
	APawn* MyPawn = GetPawn();
	if (!Actor || Actor == MyPawn)
		return;

	//Ignore other AI
	APawn* DetectedPawn = Cast<APawn>(Actor);
	if (!DetectedPawn || !DetectedPawn->IsPlayerControlled())
		return;

	//Handle sense logic
	if (Stimulus.WasSuccessfullySensed())
	{
		CanSeePlayer = true;
		TargetPlayer = Actor; // Cache the player target here
		ChasePlayer(Actor);
	}
	else
	{
		CanSeePlayer = false;
		TargetPlayer = nullptr; // Clear the target
		StopMovement();
	}
}

void ABaseAIController::ChasePlayer(AActor* Target)
{

	if (Target)
	{
		MoveToActor(Target, 25.f, true, true, true, 0, true); // 100 = acceptance radius
	}
}



