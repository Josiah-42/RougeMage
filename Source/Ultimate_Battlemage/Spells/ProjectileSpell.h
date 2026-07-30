// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseSpell.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "ProjectileSpell.generated.h"

// Forward declarations (prevents circular includes)
//class UProjectileMovementComponent;
//class USphereComponent;

/**
 * 
 */
UCLASS()
class ULTIMATE_BATTLEMAGE_API AProjectileSpell : public ABaseSpell
{
	GENERATED_BODY()
	
public:

	AProjectileSpell();

	UPROPERTY(EditAnywhere, Category = "Componenets")
	UStaticMeshComponent* ProjectileMesh;

	UPROPERTY(EditAnywhere, Category = "Componenets")
	UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY(EditAnywhere, Category = "Componenets")
	USphereComponent* Collision;



protected:

	virtual void ExecuteCast(const FSpellCastContext& Context) override;

	UFUNCTION()
	virtual void OnHit(UPrimitiveComponent* HitComp, AActor* HitActor,
		UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);


};
