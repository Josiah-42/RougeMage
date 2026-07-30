// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EStats.h"
#include "FStats.generated.h"

USTRUCT(BlueprintType)
struct FStatsStruct
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stats")
	float Health = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float MaxHealth = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float Mana = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float MaxMana = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float Stamina = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float MaxStamina = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float WalkSpeed = 600.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float SprintSpeed = 600.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float CastSpeed = 1.0f;

	// --- Helper Functions ---
	float GetStat(EStats Type) const
	{
		switch (Type)
		{
		case EStats::Health:		return Health;
		case EStats::Mana:			return Mana;
		case EStats::Stamina:		return Stamina;
		case EStats::WalkSpeed:		return WalkSpeed;
		case EStats::SprintSpeed:	return SprintSpeed;
		case EStats::CastSpeed:		return CastSpeed;
		default:					return 0.f;
		}
	}
};