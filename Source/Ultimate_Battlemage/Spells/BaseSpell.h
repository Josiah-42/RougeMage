// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SpellCastContext.h"
#include "GameFramework/Actor.h"
#include "BaseSpell.generated.h"

UCLASS()
class ULTIMATE_BATTLEMAGE_API ABaseSpell : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseSpell();

	UPROPERTY(EditDefaultsOnly, Category = "Spell|Cost")
	float ManaCost;

	UPROPERTY(EditDefaultsOnly, Category = "Spell|Cost")
	float Cooldown;

	UPROPERTY(EditDefaultsOnly, Category = "Spell|Stats")
	float Damage;

	UPROPERTY(EditDefaultsOnly, Category = "Spell|Stats")
	float ProjectilSpeed;

	UPROPERTY(EditDefaultsOnly, Category = "Spell|Stats")
	float Range;

	UPROPERTY(EditDefaultsOnly, Category = "Spell|Stats")
	float Radius;

	UPROPERTY(EditDefaultsOnly, Category = "Spell|Stats")
	float Duration;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual bool CanCast(const FSpellCastContext& Context);
	virtual void ExecuteCast(const FSpellCastContext& Context);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	bool TryCast(const FSpellCastContext& Context);


};
