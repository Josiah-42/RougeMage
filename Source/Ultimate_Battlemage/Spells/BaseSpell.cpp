// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseSpell.h"

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

bool ABaseSpell::CanCast(const FSpellCastContext& Context)
{
	return Context.Caster != nullptr && GetWorld() != nullptr;
}

void ABaseSpell::ExecuteCast(const FSpellCastContext& Context)
{
	UE_LOG(LogTemp, Warning, TEXT("Spell Executed"));
}
