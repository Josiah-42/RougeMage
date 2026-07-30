// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SpellCastContext.generated.h"

//Forward Declaration
class AActor;

/**
 * 
 */
USTRUCT(BlueprintType)
struct FSpellCastContext
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Context")
	APawn* Caster;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Context")
	FVector Origin;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Context")
	FRotator AimRotation;
};
