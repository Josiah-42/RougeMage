// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DrawDebugHelpers.h" // For visualizing the trace
#include "Engine/World.h"      // For accessing the UWorld functions
#include "CollisionQueryParams.h" // For FCollisionQueryParams
#include "Engine/EngineTypes.h" // Contains FHitResult definition (though HitResult.h might work too)
#include "BaseSpell.h"
#include "TraceSpell.generated.h"

/**
 * 
 */
UCLASS()
class ULTIMATE_BATTLEMAGE_API ATraceSpell : public ABaseSpell
{
	GENERATED_BODY()

public:

	ATraceSpell();

protected:

	virtual void ExecuteCast(const FSpellCastContext& Context) override;

	UFUNCTION()
	virtual void OnHit(UPrimitiveComponent* HitComp, AActor* HitActor,
		UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	void HandleTraceHit(const FHitResult& Hit);
	
};
