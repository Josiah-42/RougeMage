// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "BasePlayerController.h" 



//Stamina Vars
FTimerHandle StaminaDrainTimerHandle;
float StaminaDrainRate = 1.f;
float StaminaRegenRate = .5f;

//Mana Vars
float ManaRegenRate = .1f;

//Vars
FTimerHandle DelayTimeStamina;
FTimerHandle DelayTimeMana;

// Sets default values
ABaseCharacter::ABaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// --- Camera setup ---
	//CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	//CameraBoom->SetupAttachment(RootComponent);
	//CameraBoom->TargetArmLength = 300.f;
	//CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	//FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->SetupAttachment(GetMesh(), TEXT("head"));
	FVector CameraOffsetLocation = FVector(10.f, 20.f, 0.f);
	FRotator CameraOffsetRotation = FRotator(0.f, 0.f, 90.f);
	FollowCamera->SetRelativeLocationAndRotation(CameraOffsetLocation, CameraOffsetRotation);
	FollowCamera->bUsePawnControlRotation = true;

	//Force the player body model to rotate left/right (Yaw) with the camera
	bUseControllerRotationYaw = true;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;

	//Stop the character body from snapping to face the direction they are walking
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->bOrientRotationToMovement = false;
	}

	// --- Character movement defaults ---
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 500.f, 0.f);
	GetCharacterMovement()->MaxWalkSpeed = CharacterStats.WalkSpeed	;//WalkSpeed;

	// --- Character stats defaults ---
	Health = CharacterStats.Health;
	MaxHealth = CharacterStats.MaxHealth;
	Mana = CharacterStats.Mana;
	MaxMana = CharacterStats.MaxMana;
	Stamina = CharacterStats.Stamina;
	MaxStamina = CharacterStats.MaxStamina;

	// --- Script Grabs ---
	
}

// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TArray<int32> MultiOfFive = { 5, 10, 15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 65, 70, 75, 80, 85, 90, 95, 100 };
	
	for (int i = 0; i <= 100; i++)
	{
		if (MultiOfFive.Contains(i))				
		{
			RegenMana(DeltaTime);
		}
	}
}

#pragma region Controls
// Called to bind functionality to input
void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EI = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (MoveAction)
			EI->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ABaseCharacter::Move);

		if (LookAction)
			EI->BindAction(LookAction, ETriggerEvent::Triggered, this, &ABaseCharacter::Look);

		if (JumpAction)
		{
			EI->BindAction(JumpAction, ETriggerEvent::Started, this, &ABaseCharacter::Jump);
			EI->BindAction(JumpAction, ETriggerEvent::Completed, this, &ABaseCharacter::StopJumping);
		}

		if (SprintAction)
		{
			EI->BindAction(SprintAction, ETriggerEvent::Started, this, &ABaseCharacter::StartSprint);
			EI->BindAction(SprintAction, ETriggerEvent::Completed, this, &ABaseCharacter::StopSprint);
			EI->BindAction(SprintAction, ETriggerEvent::Canceled, this, &ABaseCharacter::StopSprint);
		}

		if (CastAction)
		{
			EI->BindAction(CastAction, ETriggerEvent::Triggered, this, &ABaseCharacter::CastPrimary);
		}

		if (ProjAction)
		{
			EI->BindAction(ProjAction, ETriggerEvent::Triggered, this, &ABaseCharacter::ProjCast);
		}
		
		if (TraceAction)
		{
			EI->BindAction(TraceAction, ETriggerEvent::Triggered, this, &ABaseCharacter::TraceCast);
		}

		if (AOEAction)
		{
			EI->BindAction(AOEAction, ETriggerEvent::Triggered, this, &ABaseCharacter::AOEUpdateCast);
			EI->BindAction(AOEAction, ETriggerEvent::Started, this, &ABaseCharacter::AOEStartCast);
			EI->BindAction(AOEAction, ETriggerEvent::Completed, this, &ABaseCharacter::AOEReleaseCast);

			if (AOEExtend)
			{
				EI->BindAction(AOEExtend, ETriggerEvent::Started, this, &ABaseCharacter::IncreaseAOERange);
			}

			if (AOERetract)
			{
				EI->BindAction(AOERetract, ETriggerEvent::Started, this, &ABaseCharacter::DecreaseAOERange);
			}
		}

	}
}

void ABaseCharacter::Move(const FInputActionValue& Value)
{
	//UE_LOG(LogTemp, Warning, TEXT("Move() triggered"));
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector Forward = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector Right = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(Forward, MovementVector.Y);
		AddMovementInput(Right, MovementVector.X);
	}
}

void ABaseCharacter::Look(const FInputActionValue& Value)
{
	//UE_LOG(LogTemp, Warning, TEXT("Look() triggered"));
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller)
	{
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}

	APlayerController* PC = Cast<APlayerController>(GetController());

	if (PC && PC->PlayerCameraManager)
	{
		PC->PlayerCameraManager->ViewPitchMin = -60.f;
		PC->PlayerCameraManager->ViewPitchMax = 80.f;
	}
}
#pragma endregion

#pragma region Sprint
void ABaseCharacter::StartSprint()
{
	if (Stamina >= 1)
	{
		bIsSprintg = true;
		GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
	}

	//Set timer to repeatedly call function
	if (Stamina > 0)
	{
		GetWorldTimerManager().ClearTimer(DelayTimeStamina);
		GetWorldTimerManager().ClearTimer(StaminaDrainTimerHandle);
		GetWorldTimerManager().SetTimer(StaminaDrainTimerHandle, this, &ABaseCharacter::DrainStamina, 0.1f, true);
	}
}

void ABaseCharacter::StopSprint()
{
	bIsSprintg = false;
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	GetWorldTimerManager().SetTimer(DelayTimeStamina, this, &ABaseCharacter::RegenStamina, .1f, true, 1.f);

	//Clear timer
	GetWorldTimerManager().ClearTimer(StaminaDrainTimerHandle);
}
#pragma endregion

#pragma region Stamina
void ABaseCharacter::DrainStamina()
{
	Stamina -= StaminaDrainRate;

	// Stop Sprinting with 0 Stam
	if (Stamina <= 0)
	{
		Stamina = 0;
		StopSprint();
	}

	if (ABasePlayerController* PC = Cast<ABasePlayerController>(GetController()))
	{
		PC->UpdateHUDStamina(Stamina, MaxStamina);
	}
}

void ABaseCharacter::RegenStamina()
{
	/*GetWorldTimerManager().SetTimer(DelayTime, this, &ABaseCharacter::TimeDelay, 3.f, false);*/


	if (Stamina != MaxStamina)
		Stamina += StaminaRegenRate;
	else
		GetWorldTimerManager().ClearTimer(DelayTimeStamina);

	if (ABasePlayerController* PC = Cast<ABasePlayerController>(GetController()))
	{
		PC->UpdateHUDStamina(Stamina, MaxStamina);
	}
}
#pragma endregion

#pragma region Mana
void ABaseCharacter::RegenMana(float Dt)
{
	if (Mana < MaxMana)
		Mana += ManaRegenRate * Dt;
	else
		GetWorldTimerManager().ClearTimer(DelayTimeMana);

	UpdateHud(Mana, MaxMana);
}
#pragma endregion

#pragma region Hud
void ABaseCharacter::UpdateHud(float Var, float MaxVar)
{
	if (ABasePlayerController* PC = Cast<ABasePlayerController>(GetController()))
	{
		if (Var == Mana)
			PC->UpdateHUDMana(Var, MaxVar);
		else if (Var == Stamina)
			PC->UpdateHUDStamina(Var, MaxVar);
		else
			PC->UpdateHUDHealth(Var, MaxVar);
	}
}
#pragma endregion

#pragma region BaseCast
void ABaseCharacter::CastPrimary()
{
	//ABasePlayerController* PC = Cast<ABasePlayerController>(GetController());
	
	

	// Mana Check
	if (Mana < 5.f)
		return;

	Mana -= 5.f;

	// Cast Context -- Where spell is spawned and aimed

	FVector EyeLocation;
	FRotator EyeRotation;

	// Get Location and Rotation of character Eyes
	GetActorEyesViewPoint(EyeLocation, EyeRotation);

	// Forward offset
	FVector ForwardOffset = EyeRotation.Vector() * 150.f;

	// Vertical offset (e.g., -20 units lower)
	FVector VerticalOffset = FVector(0.f, 0.f, -5.f);

	Context.Caster = this;
	Context.Origin = EyeLocation + ForwardOffset + VerticalOffset;
	Context.AimRotation = EyeRotation;

	//FHitResult hit;

	/*Context.Origin = GetActorLocation()+ GetControlRotation().Vector() * 150.f;
	Context.AimRotation = GetControlRotation();*/

	// Spawn Spell
	if (PrimarySpellClass)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, hit.GetActor()->GetFName().ToString());

		APlayerController* PC = Cast<APlayerController>(Context.Caster->GetController());
		if (PC)
		{
			FVector CameraLocation;
			FRotator CameraRotation;
			PC->GetPlayerViewPoint(CameraLocation, CameraRotation);

			FVector Start = CameraLocation;
			FVector End = Start + (CameraRotation.Vector() * 10000.0f); // 10000cm range

			// Perform LineTrace or Spawn Projectile from here
		}


		//FVector start = Context.Caster->GetActorLocation();
		//FVector forward = Context.Caster->GetController()->GetControlRotation().Vector();
		//start = FVector(start.X + (forward.X * 100), start.Y + (forward.Y * 100), start.Z + (forward.Z * 100));
		//FVector end = start + (forward * 1000);

		//FHitResult hit2;
		//FCollisionQueryParams Params(SCENE_QUERY_STAT(AOETarget), false);
		//Params.AddIgnoredActor(Context.Caster);

		//bool bHit = GetWorld()->LineTraceSingleByChannel(hit2, start, end, ECC_Visibility, Params);
		////DrawDebugLine(GetWorld(), start, end, FColor::Red, false, 2.f, 0.f, 10.f);

		//SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		//TargetPoint = bHit ? hit2.ImpactPoint : end;
		////DrawDebugLine(GetWorld(), start, end, FColor::Red, false, 2.f, 0.f, 10.f);

		//DrawDebugLine(GetWorld(), start, TargetPoint, FColor::Red, false, 2.f, 0, 2.f);
		//DrawDebugSphere(GetWorld(), TargetPoint, 25.f, 12, FColor::Red, false, 2.f);


		// MAYBE FUTURE ABILITY -CAUSES SPELL TO SPAWN AT TARGET POINT WHICH IS SET LIKE AN AOE SPELL-
		/*Spell = GetWorld()->SpawnActor<ABaseSpell>(
		PrimarySpellClass,
		TargetPoint,
		Context.AimRotation );*/

		Spell = GetWorld()->SpawnActor<ABaseSpell>(
		PrimarySpellClass,
		Context.Origin,
		Context.AimRotation );


		if (!Spell)
		{
			UE_LOG(LogTemp, Error, TEXT("SpawnActor FAILED. PrimarySpellClass=%s"),
				*GetNameSafe(PrimarySpellClass));
			return;
		}

		//Collision Debug
		/*UE_LOG(LogTemp, Warning, TEXT("Spawned Spell: %s at %s"),
			*Spell->GetName(), *Spell->GetActorLocation().ToString());*/

		if (Spell)
		{
			//Spell->SetInstigator(this);
			Spell->SetOwner(this);
			Spell->TryCast(Context);
			UE_LOG(LogTemp, Warning, TEXT("Spell Cast"));
		}
	}

	// Update Hud
	if (ABasePlayerController* PC = Cast<ABasePlayerController>(GetController()))
	{
		PC->UpdateHUDMana(Mana, MaxMana);
	}
}
#pragma endregion

#pragma region ProjectileSpell
void ABaseCharacter::ProjCast()
{
	// Mana Check
	if (Mana < 5.f)
		return;

	Mana -= 5.f;

	//

	// Cast Context -- Where spell is spawned and aimed
	FVector EyeLocation;
	FRotator EyeRotation;

	// --- Building Context ---
	// Get Location and Rotation of character Eyes
	GetActorEyesViewPoint(EyeLocation, EyeRotation);

	// Forward offset
	FVector ForwardOffset = EyeRotation.Vector() * 150.f;

	// Vertical offset (e.g., -20 units lower)
	FVector VerticalOffset = FVector(0.f, 0.f, -5.f);

	Context.Caster = this;
	Context.Origin = EyeLocation + ForwardOffset + VerticalOffset;
	Context.AimRotation = EyeRotation;
	// ---

	if (ProjSpellClass)
	{
		#pragma region Useless

		//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, hit.GetActor()->GetFName().ToString());

		// --- Useless here ?? ---
		//APlayerController* PC = Cast<APlayerController>(Context.Caster->GetController());
		//if (PC)
		//{
		//	FVector CameraLocation;
		//	FRotator CameraRotation;
		//	PC->GetPlayerViewPoint(CameraLocation, CameraRotation);

		//	FVector Start = CameraLocation;
		//	FVector End = Start + (CameraRotation.Vector() * 10000.0f); // 10000cm range
		//}
		// ---
		#pragma endregion


		// --- Spawn Spell ---
		Spell = GetWorld()->SpawnActor<ABaseSpell>(
			ProjSpellClass,
			Context.Origin,
			Context.AimRotation);
		// ---

		if (!Spell)
		{
			UE_LOG(LogTemp, Error, TEXT("SpawnActor FAILED. PrimarySpellClass=%s"),
				*GetNameSafe(PrimarySpellClass));
			return;
		}

		// --- Activate Spell ---
		if (Spell)
		{
			//Spell->SetInstigator(this);
			Spell->SetOwner(this);
			Spell->TryCast(Context);
			UE_LOG(LogTemp, Warning, TEXT("Spell Cast"));
		}
	}

	// Update Hud
	if (ABasePlayerController* PC = Cast<ABasePlayerController>(GetController()))
	{
		PC->UpdateHUDMana(Mana, MaxMana);
	}
}
#pragma endregion

#pragma region TraceSpell
void ABaseCharacter::TraceCast()
{
	// Mana Check
	if (Mana < 5.f)
		return;

	Mana -= 5.f;

	//

	Context.Caster = this;

	/*FVector start = Context.Caster->GetActorLocation();
	FVector forward = Context.Caster->GetController()->GetControlRotation().Vector();
	start += forward * 100.f;
	FVector end = start + (forward * 1000);

	FHitResult hit;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	bool bHit = GetWorld()->LineTraceSingleByChannel(hit, start, end, ECC_Visibility, Params);

	TargetPoint = bHit ? hit.ImpactPoint : end;*/

	//DrawDebugLine(GetWorld(), start, TargetPoint, FColor::Red, false, 2.f, 0, 2.f);

	// --- Update Context ---
	FVector EyeLocation;
	FRotator EyeRotation;

	// Get Location and Rotation of character Eyes
	GetActorEyesViewPoint(EyeLocation, EyeRotation);

	// Forward offset
	FVector ForwardOffset = EyeRotation.Vector() * 150.f;

	// Vertical offset (e.g., -20 units lower)
	FVector VerticalOffset = FVector(0.f, 0.f, -5.f);

	// --- EVERY CAST MUST HAVE THIS ---
	Context.Caster = this;
	Context.Origin = EyeLocation + ForwardOffset + VerticalOffset;
	Context.AimRotation = EyeRotation;
	//

	if (TraceSpellClass)
	{

		// --- Spawn Spell ---
		Spell = GetWorld()->SpawnActor<ABaseSpell>(
			TraceSpellClass,
			Context.Origin,
			Context.AimRotation);

		// --- Activate Spell ---
		if (Spell)
		{
			Spell->SetOwner(this);
			Spell->TryCast(Context);
			UE_LOG(LogTemp, Warning, TEXT("Spell Cast"));
		}
	}
}
#pragma endregion


#pragma region AOESpell


void ABaseCharacter::AOEStartCast()
{
	bIsTargeting = true;
}

void ABaseCharacter::AOEUpdateCast()
{
	if (!bIsTargeting) return;

	Context.Caster = this;
	Context.AimRotation = GetControlRotation();

	FVector start = GetPawnViewLocation();
	FRotator ViewRotation = GetControlRotation();
	FVector forward = ViewRotation.Vector();
	start += forward * 100.f;
	end = start + (forward * AOERange);

	/*FVector start = Context.Caster->GetActorLocation();
	FVector forward = Context.Caster->GetController()->GetControlRotation().Vector();
	start += forward * 100.f;
	start.Y += 50.f;
	end = start + (forward * AOERange);*/

	UE_LOG(LogTemp, Warning, TEXT("Start = %s | End = %s"),
		*start.ToString(),
		*end.ToString());

	FHitResult hit;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);



	//bool bHit = GetWorld()->LineTraceSingleByChannel(hit, start, end, ECC_Visibility, Params);

	bool bHit = GetWorld()->SweepSingleByChannel(
		hit,
		start,
		end,
		FQuat::Identity,
		ECC_Visibility,
		FCollisionShape::MakeSphere(30.f),
		Params
	);

	TargetPoint = bHit ? hit.ImpactPoint : end;

	DrawDebugLine(GetWorld(), start, TargetPoint, FColor::Red, false, 0.f, 0, 2.f);
	DrawDebugSphere(GetWorld(), TargetPoint, 25.f, 12, FColor::Red, false, 0.f);
}

void ABaseCharacter::AOEReleaseCast()
{
	// Mana Check
	if (Mana < 5.f) return;
	Mana -= 5.f;

	//
	if (!bIsTargeting) return;

	bIsTargeting = false;

	if (AOESpellClass)
	{
		FActorSpawnParameters Params;
		Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		Spell = GetWorld()->SpawnActor<ABaseSpell>(
			AOESpellClass,
			TargetPoint,
			FRotator::ZeroRotator,
			Params
		);

		UE_LOG(LogTemp, Warning, TEXT("AOE Spawned"));

		if (Spell)
		{
			Context.Caster = this;
			Context.Origin = TargetPoint;
			Spell->TryCast(Context);
		}

		
	}
}

void ABaseCharacter::IncreaseAOERange()
{
	AOERange = FMath::Clamp(AOERange + 100.f, 200.f, 1000.f);
}

void ABaseCharacter::DecreaseAOERange()
{
	AOERange = FMath::Clamp(AOERange - 100.f, 200.f, 1000.f);
}
#pragma endregion



