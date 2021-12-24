#include "CThrow_KnockBack.h"
#include "Global.h"
#include "Components/SphereComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "CSkill.h"

#include "Abilities/CAbility.h"
#include "Abilities/CAbility_Projectile.h"

ACThrow_KnockBack::ACThrow_KnockBack()
{
	PrimaryActorTick.bCanEverTick = true;

	SphereCollision = CreateDefaultSubobject<USphereComponent>("SphereCollision");

	ParticleSystem = CreateDefaultSubobject<UParticleSystemComponent>("ParticleSystem");
	ParticleSystem->SetupAttachment(SphereCollision);

	ProjectileComponent = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileComponent");
	ProjectileComponent->ProjectileGravityScale = 0.0f;

	ProjectileComponent->InitialSpeed = 250.0f;
	ProjectileComponent->MaxSpeed = 400.0f;

	LifeTime = 0.0f;
	KnockBackPower = 1000.0f;
}

void ACThrow_KnockBack::BeginPlay()
{
	Super::BeginPlay();


	SetLifeSpan(LifeTime);
	
	if (OwnerSkill != NULL)
		OwnerSkill->SetKnockBackPower(KnockBackPower);
}

void ACThrow_KnockBack::EndOverlap(AActor * InOtherActor)
{
	ACharacter* character = Cast<ACharacter>(InOtherActor);
	HitActors.Remove(character);
}

void ACThrow_KnockBack::Reset()
{
	HitActors.Empty();
}


