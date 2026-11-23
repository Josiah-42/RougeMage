// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseSpell.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ABaseSpell::ABaseSpell()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABaseSpell::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABaseSpell::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool ABaseSpell::TryCast(const FSpellCastContext& Context)
{
	if (!CanCast(Context)) return false;

	ExecuteCast(Context);
	return true;
}

FVector ABaseSpell::CalculateSpawnOrigin(const FVector& DefaultOrigin, AActor* Target) const
{
	// Default: Spawn front of AI eyes
	return DefaultOrigin;
}

FRotator ABaseSpell::CalculateAimRotation(const FVector& SpawnOrigin, AActor* Target) const
{
	// Default: Aim straight at target (TraceSpell)
	if (!Target) return FRotator::ZeroRotator;
	return UKismetMathLibrary::FindLookAtRotation(SpawnOrigin, Target->GetActorLocation());
}

bool ABaseSpell::CanCast(const FSpellCastContext& Context)
{
	return Context.Caster != nullptr && GetWorld() != nullptr;
}

void ABaseSpell::ExecuteCast(const FSpellCastContext& Context)
{
	UE_LOG(LogTemp, Warning, TEXT("Spell Executed"));
}
