// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BasePlayerController.generated.h"

/**
 * 
 */

// Forward declarations
class UInputMappingContext;
class UPlayerHUDWidget;
//class ABaseCharacter;


UCLASS()
class ULTIMATE_BATTLEMAGE_API ABasePlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ABasePlayerController();

protected: 
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;

public:
	// --- Input SetUp ---
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputMappingContext* InputMappingContext;

	// --- UI Setup ---
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UPlayerHUDWidget> HUDWidgetClass;

	UPROPERTY(BlueprintReadOnly, Category = "UI")
	UPlayerHUDWidget* HUDWidget;

	UPROPERTY(EditDefaultsOnly, Category = "MyPawn")
	APawn* MyPawn;

	// Called when players health or mana change
	void UpdateHUDHealth(float CurrentHealth, float MaxHealth);
	void UpdateHUDMana(float CurrentMana, float MaxMana);
	void UpdateHUDStamina(float CurrentStamina, float MaxStamina);
	
};
