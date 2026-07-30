// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Estats.generated.h"

UENUM(BlueprintType)
enum class EStats : uint8
{
	Health,
	Mana,
	Stamina,
	WalkSpeed,
	SprintSpeed,
	CastSpeed
};
