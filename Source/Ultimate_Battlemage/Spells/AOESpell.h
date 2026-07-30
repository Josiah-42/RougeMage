// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "BaseSpell.h"
#include "Engine/World.h"
#include "EngineMinimal.h"
#include "AOESpell.generated.h"

/**
 * 
 */
UCLASS()
class ULTIMATE_BATTLEMAGE_API AAOESpell : public ABaseSpell
{
	GENERATED_BODY()
	
public:
	
	AAOESpell();

	UPROPERTY(EditAnywhere, Category = "Componenets")
	UStaticMeshComponent* ProjectileMesh;

	UPROPERTY(EditAnywhere, Category = "Componenets")
	USphereComponent* Collision;

	bool AOESpellOn = false;
	FVector TargetPoint;
	FActorSpawnParameters SpawnParams;
	AActor* Caster;
	FSpellCastContext Context2;


protected:

	virtual void ExecuteCast(const FSpellCastContext& Context) override;

	/*UFUNCTION()
	virtual void OnHit(UPrimitiveComponent* HitComp, AActor* HitActor,
		UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);*/

	UFUNCTION()
	virtual void OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
