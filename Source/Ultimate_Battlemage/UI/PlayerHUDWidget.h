// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerHUDWidget.generated.h"

class UProgressBar;
class UTextBlock;

/**
 *
 */
UCLASS()
class ULTIMATE_BATTLEMAGE_API UPlayerHUDWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	
	UFUNCTION()
	virtual void NativeConstruct() override;

	// Called to update health bar percent (0.0 - 1.0)
	UFUNCTION(BlueprintCallable, Category = "HUD")
	void SetHealthPercent(float NewPercent);

	// Called to update mana bar percent (0.0 - 1.0)
	UFUNCTION(BlueprintCallable, Category = "HUD")
	void SetManaPercent(float NewPercent);

	// Called to update mana bar percent (0.0 - 1.0)
	UFUNCTION(BlueprintCallable, Category = "HUD")
	void SetStaminaPercent(float NewPercent);

protected:
	// References to your progress bar (bind these in your widget blueprint)
	UPROPERTY(meta = (BindWidget))
	UProgressBar* HealthBar;

	UPROPERTY(meta = (BindWidget))
	UProgressBar* ManaBar;

	UPROPERTY(meta = (BindWidget))
	UProgressBar* StaminaBar;

	// Optional: text blacks to display numeric values
	UPROPERTY(meta = (BindWidgetOptional))
	UTextBlock* HealthText;

	UPROPERTY(meta = (BindWidgetOptional))
	UTextBlock* ManaText;

	UPROPERTY(meta = (BindWidgetOptional))
	UTextBlock* StaminaText;
	
};
