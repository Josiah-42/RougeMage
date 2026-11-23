// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileSpell.h"
#include "DrawDebugHelpers.h"
#include "../AI/BaseAI.h"
#include "../Player/BaseCharacter.h"
#include "Kismet/KismetMathLibrary.h"

AProjectileSpell::AProjectileSpell()
{
	PrimaryActorTick.bCanEverTick = false;

	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Projectile Mesh"));
	Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	//RootComponent = ProjectileMesh;

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>("Movement");
	ProjectileMovement->SetUpdatedComponent(Collision);
	ProjectileSpeed = 2000.f;
	ProjectileMovement->InitialSpeed = ProjectileSpeed                                                                                                                                                                                                                                                                                        ;
	ProjectileMovement->MaxSpeed = 10000.f;
	ProjectileMovement->ProjectileGravityScale = 0.5f;
	//ProjectileMovement->Velocity = GetActorForwardVector() * 3000.f;
	InitialLifeSpan = 5.f;

	//Collision->SetupAttachment(ProjectileMesh);
	
	Collision->InitSphereRadius(10.f);
	Collision->SetCollisionProfileName("BlockAll");
	Collision->SetNotifyRigidBodyCollision(true);
	Collision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Collision->SetCollisionResponseToAllChannels(ECR_Ignore);
	Collision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	Collision->SetGenerateOverlapEvents(true);

	Collision->OnComponentHit.AddDynamic(this, &AProjectileSpell::OnHit);

	RootComponent = Collision;
	ProjectileMesh->SetupAttachment(Collision);

}

FRotator AProjectileSpell::CalculateAimRotation(const FVector& SpawnOrigin, AActor* Target) const
{
	if (!Target) return FRotator::ZeroRotator;

	FVector TargetLocation = Target->GetActorLocation();
	FVector TargetVelocity = Target->GetVelocity();

	// Lead the shot using BSpell PSpell variable
	float Distance = FVector::Dist(SpawnOrigin, TargetLocation);
	float TimeToImpact = (ProjectileSpeed > 0.f) ? (Distance / ProjectileSpeed) : 0.f;
	FVector PredictedLocation = TargetLocation + (TargetVelocity * TimeToImpact);

	// Account for gravity 
	float GravityZ = GetWorld() ? GetWorld()->GetGravityZ() : -980.f;
	float GravityScale = ProjectileMovement ? ProjectileMovement->ProjectileGravityScale : .5f;
	float GravityDrop = .5f * (GravityZ * GravityScale) * FMath::Square(TimeToImpact);
	PredictedLocation.Z -= GravityDrop;

	return UKismetMathLibrary::FindLookAtRotation(SpawnOrigin, PredictedLocation);
}

void AProjectileSpell::ExecuteCast(const FSpellCastContext& Context)
{
	//SetActorLocation(Context.Origin);
	//SetActorRotation(Context.AimRotation);

	// ???

	ProjectileMovement->Velocity = Context.AimRotation.Vector() * ProjectileSpeed;
	
	UE_LOG(LogTemp, Warning, TEXT("Casting Projectile Spell"));
	DrawDebugSphere(GetWorld(), GetActorLocation(), 20.f, 12, FColor::Green, false, 2.f);
}


void AProjectileSpell::OnHit(UPrimitiveComponent* HitComp, AActor* HitActor,
	UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	UE_LOG(LogTemp, Warning, TEXT("Projectile HIT: HitActor=%s"), *GetNameSafe(HitActor));
	ABaseAI* Enemy = Cast<ABaseAI>(HitActor); // Replace with your actual AI class name
	ABaseCharacter* Player = Cast<ABaseCharacter>(HitActor);

	if (Enemy)
	{
		Enemy->CurrHealth -= 10.0f;

		Enemy->UpdateHealthBarVisuals();

		if (Enemy->CurrHealth <= 0.f)
		{
			HitActor->Destroy();
		}
	}

	if (Player)
	{
		Player->CurrHealth -= 10.0f;

		Player->UpdateHud(Player->CurrHealth, Player->MaxHealth);

		if (Player->CurrHealth <= 0.f)
		{
			HitActor->Destroy();
		}
	}
	Destroy();
}
