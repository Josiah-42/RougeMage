// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
//#include "NiagaraSystem.h"
#include "ProjectileSpell.h"
#include "FireBall.generated.h"

/**
 * 
 */
UCLASS()
class ULTIMATE_BATTLEMAGE_API AFireBall : public AProjectileSpell
{
	GENERATED_BODY()

public:

	AFireBall();

protected:

	UPROPERTY(EditDefaultsOnly, Category = "Spell|Fireball")
	float ExplosionRadius = 250.f;

	//UPROPERTY(EditDefaultsOnly, Category = "Spell|Fireball")
	
protected:

	virtual void ExecuteCast(const FSpellCastContext& Context) override;
	
};
