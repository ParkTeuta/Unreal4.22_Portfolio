#include "CThrow_WaterBoom.h"
#include "Global.h"
#include "Components/SphereComponent.h"

#include "Particles/ParticleSystemComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "CSkill.h"

#include "Abilities/CAbility.h"
#include "Abilities/CAbility_Projectile.h"

#include "Sound/SoundBase.h"

ACThrow_WaterBoom::ACThrow_WaterBoom()
{
	PrimaryActorTick.bCanEverTick = true;

	SphereCollision = CreateDefaultSubobject<USphereComponent>("SphereCollision");

	ParticleSystem = CreateDefaultSubobject<UParticleSystemComponent>("ParticleSystem");
	ParticleSystem->SetupAttachment(SphereCollision);

	ProjectileComponent = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileComponent");
	
	Dist = 10.0f;
	Volume = 0.8f;

	AirbornePower = 600.0f;
}

void ACThrow_WaterBoom::BeginPlay()
{
	Super::BeginPlay();

}

void ACThrow_WaterBoom::Tick(float DeltaTime)
{
	FVector pos = GetActorLocation();
	if (FVector::Distance(pos, TargetPosition) <= Dist)
		HitPlane();
}

void ACThrow_WaterBoom::EndOverlap(AActor * InOtherActor)
{
	ACharacter* character = Cast<ACharacter>(InOtherActor);
	HitActors.Remove(character);
}

void ACThrow_WaterBoom::SetHitActors(AActor * InActor)
{
	ACharacter* character = Cast<ACharacter>(InActor);
	CheckNull(character);
	ExistHitActor(character);
}

void ACThrow_WaterBoom::HitPlane()
{
	CheckNull(GetOwnerSkill());

	if (PlaneSound != NULL)
		UGameplayStatics::SpawnSoundAtLocation(GetWorld(), PlaneSound, TargetPosition, FRotator::ZeroRotator, Volume);

	for (ACharacter* character : HitActors)
	{
		if (CanDameged(character) == false)
			continue;

		ACharacter* ownerCharacter = GetOwnerCharacter();

		Power = GetOwnerSkill()->GetPower();

		FDamageEvent e;
		character->TakeDamage(Power, e, ownerCharacter->GetController(), this);

		if (HitParticle != NULL)
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitParticle, character->GetActorLocation());
		}


		if (GetOwnerSkill()->GetHitSound() != NULL)
			UGameplayStatics::SpawnSoundAttached(GetOwnerSkill()->GetHitSound(), character->GetRootComponent());

		character->LaunchCharacter(FVector(0, 0, 1) * AirbornePower, false, false);
	}
	DestroyDecal();
	Destroy();
}

void ACThrow_WaterBoom::SetTargetPosition(FVector InVector)
{
	TargetPosition = InVector;

	SetDecal(TargetPosition);
}

void ACThrow_WaterBoom::Reset()
{
	HitActors.Empty();
}


