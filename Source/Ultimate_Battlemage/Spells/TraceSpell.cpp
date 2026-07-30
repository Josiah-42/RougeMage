// Fill out your copyright notice in the Description page of Project Settings.


#include "TraceSpell.h"

ATraceSpell::ATraceSpell()
{
	
}

void ATraceSpell::ExecuteCast(const FSpellCastContext& Context)
{
	/*FVector start = GetActorLocation();
	FVector forward = GetActorForwardVector();
	start = FVector(start.X + (forward.X * 100), start.Y + (forward.Y * 100), start.Z + (forward.Z * 100));
	FVector end = start + (forward * 1000);*/

	FVector start = Context.Origin;
	FVector forward = Context.AimRotation.Vector();
	FVector end = start + (forward * 1000.f);

	FHitResult hit;
	FCollisionQueryParams Params;
	//Params.AddIgnoredActor(this);
	Params.AddIgnoredActor(Context.Caster);

	if (GetWorld())
	{
		bool actorHit = GetWorld()->LineTraceSingleByChannel(hit, start, end, ECC_Pawn, Params);

		FVector TargetPoint = actorHit ? hit.ImpactPoint : end;

		DrawDebugLine(GetWorld(), start, end, FColor::Red, false, 2.f, 0.f, 10.f);

		if (actorHit && hit.GetActor())
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, hit.GetActor()->GetFName().ToString());

			UPrimitiveComponent* HitComp = hit.GetComponent();

			if (HitComp)
			{
				ECollisionChannel Channel = HitComp->GetCollisionObjectType();

				if (Channel == ECC_WorldStatic || Channel == ECC_WorldDynamic)
				{
					return; // Ignore floor and environment
				}
			}

			HandleTraceHit(hit);
			//hit.GetComponent()->OnComponentHit.AddDynamic(this, &ATraceSpell::OnHit);
		}
	}



	UE_LOG(LogTemp, Warning, TEXT("Casting Trace Spell"));

}

void ATraceSpell::HandleTraceHit(const FHitResult& Hit)
{
	AActor* HitActor = Hit.GetActor();

	if (!HitActor) return;

	UE_LOG(LogTemp, Warning, TEXT("Trace HIT: HitActor=%s"), *GetNameSafe(HitActor));

	// Example: destroy hit actor (careful with player/world actors!)
	HitActor->Destroy();

	// Optional: destroy spell actor if it exists in world
	Destroy();
}

void ATraceSpell::OnHit(UPrimitiveComponent* HitComp, AActor* HitActor,
	UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	UE_LOG(LogTemp, Warning, TEXT("Trace HIT: HitActor=%s"), *GetNameSafe(HitActor));
	Destroy();
	HitActor->Destroy();
}
