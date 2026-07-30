// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileSpell.h"
#include "DrawDebugHelpers.h"

AProjectileSpell::AProjectileSpell()
{
	PrimaryActorTick.bCanEverTick = false;

	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Projectile Mesh"));
	//RootComponent = ProjectileMesh;

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>("Movement");
	ProjectileMovement->SetUpdatedComponent(Collision);
	ProjectileMovement->InitialSpeed = 2000.f;
	ProjectileMovement->MaxSpeed = 10000.f;
	ProjectileMovement->ProjectileGravityScale = 0.5f;
	//ProjectileMovement->Velocity = GetActorForwardVector() * 3000.f;
	InitialLifeSpan = 5.f;

	Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
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

void AProjectileSpell::ExecuteCast(const FSpellCastContext& Context)
{
	//SetActorLocation(Context.Origin);
	//SetActorRotation(Context.AimRotation);



	// ???

	ProjectileMovement->Velocity = Context.AimRotation.Vector() * ProjectileMovement->InitialSpeed;
	
	UE_LOG(LogTemp, Warning, TEXT("Casting Projectile Spell"));
	DrawDebugSphere(GetWorld(), GetActorLocation(), 20.f, 12, FColor::Green, false, 2.f);
}


void AProjectileSpell::OnHit(UPrimitiveComponent* HitComp, AActor* HitActor,
	UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	UE_LOG(LogTemp, Warning, TEXT("Projectile HIT: HitActor=%s"), *GetNameSafe(HitActor));
	Destroy();
	HitActor->Destroy();
}
