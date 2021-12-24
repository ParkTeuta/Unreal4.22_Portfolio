#include "CBossSkill4_ThrowItem.h"
#include "Global.h"
#include "Components/SphereComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Weapons/CSkill.h"

#include "Abilities/CAbility.h"
#include "Abilities/CAbility_Projectile.h"
#include "Characters/CPlayer.h"

#include "Components/CStateComponent.h"
#include "Components/CCrowdControlComponent.h"

ACBossSkill4_ThrowItem::ACBossSkill4_ThrowItem()
{
	PrimaryActorTick.bCanEverTick = true;

	SphereCollision = CreateDefaultSubobject<USphereComponent>("SphereCollision");

	ParticleSystem = CreateDefaultSubobject<UParticleSystemComponent>("ParticleSystem");
	ParticleSystem->SetupAttachment(SphereCollision);

	ProjectileComponent = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileComponent");
	ProjectileComponent->ProjectileGravityScale = 0.0f;

	ProjectileComponent->InitialSpeed = 250.0f;
	ProjectileComponent->MaxSpeed = 350.0f;

	LifeTime = 0.0f;
	AirbornePower = 1000.0f;
}

void ACBossSkill4_ThrowItem::BeginPlay()
{
	Super::BeginPlay();

	SetLifeSpan(LifeTime);
	
}

void ACBossSkill4_ThrowItem::Tick(float DeltaTime)
{
		
}


void ACBossSkill4_ThrowItem::SetHitActors(AActor * InActor)
{
	CheckNull(OwnerSkill);

	ACPlayer* character = Cast<ACPlayer>(InActor);
	CheckNull(character);

	if (ExistHitActor(character))
		return;

	ACharacter* ownerCharacter = OwnerSkill->GetOwnerCharacter();
	FDamageEvent e;
	Power = OwnerSkill->GetPower();
	
	if (HitParticleClass != NULL)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitParticleClass, character->GetTransform());
	}

	character->TakeDamage(Power, e, ownerCharacter->GetController(), this);

}

void ACBossSkill4_ThrowItem::Reset()
{
	HitActors.Empty();
}


