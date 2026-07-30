// Fill out your copyright notice in the Description page of Project Settings.


#include "BasePlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Blueprint/UserWidget.h"
#include "Ultimate_Battlemage/UI/PlayerHUDWidget.h"
#include "BaseCharacter.h"

ABasePlayerController::ABasePlayerController()
{

}

void ABasePlayerController::BeginPlay()
{
	Super::BeginPlay();

    // Spawn HUD When the game starts
    if (HUDWidgetClass)
    {
        HUDWidget = CreateWidget<UPlayerHUDWidget>(this, HUDWidgetClass);
        
        if (HUDWidget)
        {
            HUDWidget->AddToViewport();
        }
    }

    // --- Optional: Update HUD immediately spawning after Pawn Possess ---
    ABaseCharacter* CharacterMine = Cast<ABaseCharacter>(MyPawn);
    if (CharacterMine)
    {
        UpdateHUDHealth(CharacterMine->Health, CharacterMine->MaxHealth);
        UpdateHUDMana(CharacterMine->Mana, CharacterMine->MaxMana);
        UpdateHUDStamina(CharacterMine->Stamina, CharacterMine->MaxStamina);
    }

	//// --- Add the Input Mapping Context to this local player ---
	//if (UEnhancedInputLocalPlayerSubsystem* Subsytem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	//{
	//	if (InputMappingContext)
	//	{
	//		Subsytem->AddMappingContext(InputMappingContext, 0);
	//	}
	//}

}

void ABasePlayerController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);

    MyPawn = InPawn;

    if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
    {
        if (InputMappingContext)
        {
            Subsystem->AddMappingContext(InputMappingContext, 0);
            UE_LOG(LogTemp, Warning, TEXT("IMC Added"));
        }
    }

   
   
}

void ABasePlayerController::UpdateHUDHealth(float CurrentHealth, float MaxHealth)
{
    if (HUDWidget)
    {
        const float Percent = MaxHealth > 0 ? (CurrentHealth / MaxHealth) : 0.f;
        HUDWidget->SetHealthPercent(Percent);
    }
}

void ABasePlayerController::UpdateHUDMana(float CurrentMana, float MaxMana)
{
    if (HUDWidget)
    {
        const float Percent = MaxMana > 0 ? (CurrentMana / MaxMana) : 0.f;
        HUDWidget->SetManaPercent(Percent);
    }
}

void ABasePlayerController::UpdateHUDStamina(float CurrentStamina, float MaxStamina)
{
    if (HUDWidget)
    {
        const float Percent = MaxStamina > 0 ? (CurrentStamina / MaxStamina) : 0.f;
        HUDWidget->SetStaminaPercent(Percent);
    }
}