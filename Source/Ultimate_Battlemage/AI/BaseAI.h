// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
//#include "Perception/AIPerceptionComponent.h"
//#include "Perception/AISenseConfig_Sight.h"
#include "BaseAI.generated.h"

//class UAIPerceptionComponent;
//class UAISenseConfig_Sight;

UCLASS()
class ULTIMATE_BATTLEMAGE_API ABaseAI : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseAI();

	//AI Functions
	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	

	//AI Components
	

	//AI Variables
	
};
