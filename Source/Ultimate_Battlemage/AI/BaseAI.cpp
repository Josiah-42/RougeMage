// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseAI.h"
#include "BaseAIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/WidgetComponent.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"


// Sets default values
ABaseAI::ABaseAI()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AIControllerClass = ABaseAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	if (GetCharacterMovement())
	{
		GetCharacterMovement()->MaxWalkSpeed = 200.f;
	}

	// --- Ai stats defaults ---
	CurrHealth = AIStats.Health;
	MaxHealth = AIStats.MaxHealth;
	Mana = AIStats.Mana;
	MaxMana = AIStats.MaxMana;
	Stamina = AIStats.Stamina;
	MaxStamina = AIStats.MaxStamina;

}


// Called when the game starts or when spawned
void ABaseAI::BeginPlay()
{
	Super::BeginPlay();

	UpdateHealthBarVisuals();
	
}

void ABaseAI::UpdateHealthBarVisuals()
{
	//Widget Comp
	UWidgetComponent* WidgetComp = FindComponentByClass<UWidgetComponent>();
	if (!WidgetComp) return;

	//Widget Object
	UUserWidget* HPUserWidget = WidgetComp->GetUserWidgetObject();

	//Progress Bar
	UProgressBar* AIHPBar = Cast<UProgressBar>(HPUserWidget->GetWidgetFromName(TEXT("HealthBar")));

	if (AIHPBar)
	{
		//HP
		float HealthPercent = CurrHealth / MaxHealth;

		//Set Progress bar fill
		AIHPBar->SetPercent(HealthPercent);
	}
}




