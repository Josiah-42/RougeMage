// Fill out your copyright notice in the Description page of Project Settings.


#include "AOESpell.h"

AAOESpell::AAOESpell()
{
	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Projectile Mesh"));

	Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));

	Collision->SetCollisionProfileName("BlockAll");
	Collision->SetNotifyRigidBodyCollision(true);
	Collision->InitSphereRadius(250.f);
	//Collision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Collision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Collision->SetCollisionResponseToAllChannels(ECR_Overlap);
	Collision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	Collision->SetGenerateOverlapEvents(true);	

	Collision->OnComponentBeginOverlap.AddDynamic(this, &AAOESpell::OnOverlap);


	//Collision->OnComponentBeginOverlap.AddDynamic(this, &AAOESpell::OnHit);

	RootComponent = Collision;
	//RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	ProjectileMesh->SetupAttachment(Collision);
	ProjectileMesh->SetMobility(EComponentMobility::Movable);
}

void AAOESpell::ExecuteCast(const FSpellCastContext& Context)
{   
	// --- Active when spell is casted ---
	Caster = Context.Caster;

	Collision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	AOESpellOn = true;
	bIsCasted = true;

	if (GEngine) 
	{
		GEngine->AddOnScreenDebugMessage(
			-1,             // Unique key to prevent message flooding (-1 means add a new message)
			10.0f,           // Duration in seconds to display the message
			FColor::Cyan,   // Text color
			FString::Printf(TEXT("AOE SPELL MESH LOCATION: %s, AOE SPELL TARGETPOINT: %s"), *GetActorLocation().ToString(), *TargetPoint.ToString()));
	}

	if (bIsCasted)
	{
		SpellDestroyDelay(3.f);
	}

}

//void AAOESpell::OnHit(UPrimitiveComponent* HitComp, AActor* HitActor,
//	UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
//{
//	AOESpellOn = false;
//	UE_LOG(LogTemp, Warning, TEXT("AOE HIT: HitActor=%s"), *GetNameSafe(HitActor));
//	Destroy();
//	HitActor->Destroy();
//}

void AAOESpell::OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, 
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// --- Active when spell overlapped
	UE_LOG(LogTemp, Warning, TEXT("AOE Overlap"));
	AOESpellOn = false;
	UE_LOG(LogTemp, Warning, TEXT("AOE HIT: OtherActor=%s"), *GetNameSafe(OtherActor));
	if (OtherActor && !OtherActor->IsA(AAOESpell::StaticClass()) && OtherActor != Caster)
	{
		OtherActor->Destroy();
		Destroy();
	}
}

void AAOESpell::SpellDestroyDelay(float Durations)
{
	// Start timer delay
	GetWorldTimerManager().SetTimer(DestroyDelay, this, &AAOESpell::DestroySpell, Durations, false);
}

void AAOESpell::DestroySpell()
{
	bIsCasted = false;
	Destroy();
}