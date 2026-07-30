// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseGameMode.h"
#include "Player/BaseCharacter.h"
#include "GameFramework/PlayerController.h"

ABaseGameMode::ABaseGameMode()
{
	// --- Set the default pawn class to your custom character ---
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Blueprints/Player/BP_BaseCharacter"));
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	if (!PlayerPawnBPClass.Class)
	{
		UE_LOG(LogTemp, Warning, TEXT("Could not find BP_BaseCharacter asset!"));
	}

	//// --- Set custom controller ---
	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerBPClass(TEXT("/Game/Blueprints/Player/BP_BaseController"));
	if (PlayerControllerBPClass.Class != nullptr)
	{
		PlayerControllerClass = PlayerControllerBPClass.Class;
	}


}

