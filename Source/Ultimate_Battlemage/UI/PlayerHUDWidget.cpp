// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerHUDWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UPlayerHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();

	UE_LOG(LogTemp, Warning, TEXT("Widget Constructed - BindWidgets are now valid"));

	//// Now you can safely set initial values
	//SetHealthPercent(1.0f);
	//SetManaPercent(1.0f);
}

void UPlayerHUDWidget::SetHealthPercent(float NewPercent)
{
	if (HealthBar)
	{
		HealthBar->SetPercent(FMath::Clamp(NewPercent, 0.0f, 1.0f));

		if (HealthText)
		{
			const int32 DisplayValue = FMath::RoundToInt(NewPercent * 100.f);
			HealthText->SetText(FText::FromString(FString::Printf(TEXT("%d%%"), DisplayValue)));
			//UE_LOG(LogTemp, Warning, TEXT("Health Set"));
		}
	}
}

void UPlayerHUDWidget::SetManaPercent(float NewPercent)
{
	if (ManaBar)
	{
		ManaBar->SetPercent(FMath::Clamp(NewPercent, 0.0f, 1.0f));

		if (ManaText)
		{
			const int32 DisplayValue = FMath::RoundToInt(NewPercent * 100.f);
			ManaText->SetText(FText::FromString(FString::Printf(TEXT("%d%%"), DisplayValue)));
			//UE_LOG(LogTemp, Warning, TEXT("Mana Set"));
		}
	}
}

void UPlayerHUDWidget::SetStaminaPercent(float NewPercent)
{
	if (StaminaBar)
	{
		StaminaBar->SetPercent(FMath::Clamp(NewPercent, 0.0f, 1.0f));

		if (StaminaText)
		{
			const int32 DisplayValue = FMath::RoundToInt(NewPercent * 100.f);
			StaminaText->SetText(FText::FromString(FString::Printf(TEXT("%d%%"), DisplayValue)));
			//UE_LOG(LogTemp, Warning, TEXT("Stamina Set"));
		}
	}
}

