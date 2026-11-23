// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseAIController.h"
#include "Kismet/GameplayStatics.h"
#include "BaseAI.h"
#include "../Player/BaseCharacter.h"
#include "Kismet/KismetMathLibrary.h"

ABaseAIController::ABaseAIController()
{
	Perception = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception"));
	SetPerceptionComponent(*Perception);

	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));

	SightConfig->SightRadius = 1200.f;
	SightConfig->LoseSightRadius = 10000.f; // usually slightly bigger
	SightConfig->PeripheralVisionAngleDegrees = 360.f;
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
	myActor = Cast<ABaseAI>(GetPawn());
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
		//ChasePlayer(TargetPlayer);
		
		if (!bIsCasting)
		{
			AttackPlayer(TargetPlayer, myActor->MySpell);
		}
	}
}


void ABaseAIController::OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
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
		SetFocus(Actor); // Force AI to face player
		ChasePlayer(Actor);
		//AttackPlayer(Actor, myActor->MySpell);
	}
	else
	{
		CanSeePlayer = false;
		TargetPlayer = nullptr; // Clear the target
		ClearFocus(EAIFocusPriority::Gameplay);
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

void ABaseAIController::AttackPlayer(AActor* Target, TSubclassOf<ABaseSpell> spell)
{
	if (!Target || !spell) return;

	// Cast Context -- Where spell is spawned and aimed
	
	// Get Location and Rotation of character Eyes
	FVector EyeLocation;
	FRotator EyeRotation;
	GetActorEyesViewPoint(EyeLocation, EyeRotation);

	// Calculate spell spawn
	FVector ForwardOffset = EyeRotation.Vector() * 150.f;
	FVector VerticalOffset = FVector(0.f, 0.f, -5.f);
	FVector SpawnOrigin = EyeLocation + ForwardOffset + VerticalOffset;

	// Predict player
	float SpellSpeed = 10000.f;
	float Distance = FVector::Dist(SpawnOrigin, Target->GetActorLocation());
	float TimeToImpact = Distance / SpellSpeed;

	FVector PredictedTargetLocation = Target->GetActorLocation() + (Target->GetVelocity() * TimeToImpact);

	// Calculate rotation from spawn to predicted lo
	FRotator AimRotation = UKismetMathLibrary::FindLookAtRotation(SpawnOrigin, PredictedTargetLocation);

	/*--- Possible accuracy spread-- -

	float spreadradians = FMath::DegreesToRadians(2.f);
	AimRotation = FMath::VRandCone(AimRotation.Vector(), SpreadRadians).Rotation();

	---*/
	
	// --- Build Context with new AimRotation ---
	Context.Caster = GetPawn();
	Context.Origin = SpawnOrigin;
	Context.AimRotation = AimRotation;
	// ---

	Spell1 = GetWorld()->SpawnActor<ABaseSpell>(
		spell,
		Context.Origin,
		Context.AimRotation);

	if (!Spell1)
	{
		UE_LOG(LogTemp, Error, TEXT("SpawnActor FAILED. PrimarySpellClass=%s"),
			*GetNameSafe(spell));
		return;
	}

	// --- Activate Spell ---
	if (Spell1)
	{
		//Spell->SetInstigator(this);
		Spell1->SetOwner(myActor);
		Spell1->TryCast(Context);
		UE_LOG(LogTemp, Warning, TEXT("Spell Cast"));
	}

	StartSpellLockout(3.0);
}

void ABaseAIController::UpdateAim(float DeltaTime)
{

}

#pragma region SpellLockout
void ABaseAIController::StartSpellLockout(float Duration)
{
	bIsCasting = true;

	// Start timer delay
	GetWorldTimerManager().SetTimer(SpellDelay, this, &ABaseAIController::ResetSpellCastLockout, Duration, false);
}

void ABaseAIController::ResetSpellCastLockout()
{
	bIsCasting = false;
	UE_LOG(LogTemp, Log, TEXT("Lockout lifted. Ready for the next spell!"));
}
#pragma endregion 
