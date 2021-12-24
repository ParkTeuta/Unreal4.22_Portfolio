#include "CThrow_Void.h"
#include "Global.h"
#include "Components/SphereComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "CSkill.h"

#include "Abilities/CAbility.h"
#include "Abilities/CAbility_Projectile.h"

ACThrow_Void::ACThrow_Void()
{
	PrimaryActorTick.bCanEverTick = true;

	SphereCollision = CreateDefaultSubobject<USphereComponent>("SphereCollision");

	ParticleSystem = CreateDefaultSubobject<UParticleSystemComponent>("ParticleSystem");
	ParticleSystem->SetupAttachment(SphereCollision);

	ProjectileComponent = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileComponent");
	ProjectileComponent->ProjectileGravityScale = 0.0f;

	ProjectileComponent->InitialSpeed = 250.0f;
	ProjectileComponent->MaxSpeed = 350.0f;

	TickTime = 0.0f;
	CountDown = 0.0f;
	LifeTime = 0.0f;
}

void ACThrow_Void::BeginPlay()
{
	Super::BeginPlay();

	SetLifeSpan(LifeTime);
	
}

void ACThrow_Void::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CheckNull(GetOwnerSkill());

	if (HitActors.Num() > 0)
	{
		if (TickTime <= CountDown)
		{
			ACharacter* ownerCharacter = OwnerSkill->GetOwnerCharacter();
			for (ACharacter* character : HitActors)
			{
				if (CanDameged(character) == false)
					continue;

				if (HitParticle != NULL)
				{
					UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitParticle, character->GetActorLocation());
				}

				if (GetOwnerSkill()->GetHitSound() != NULL)
					UGameplayStatics::SpawnSoundAttached(GetOwnerSkill()->GetHitSound(), character->GetRootComponent());

				FDamageEvent e;
				Power = OwnerSkill->GetPower();
				character->TakeDamage(Power, e, ownerCharacter->GetController(), this);
			}
			CountDown = 0;
		}
		else
			CountDown += DeltaTime;
	}
}

void ACThrow_Void::EndOverlap(AActor * InOtherActor)
{
	ACharacter* character = Cast<ACharacter>(InOtherActor);
	HitActors.Remove(character);
}

void ACThrow_Void::SetHitActors(AActor * InActor)
{
	ACharacter* character = Cast<ACharacter>(InActor);
	CheckNull(character);
	ExistHitActor(character);
}

void ACThrow_Void::Reset()
{
	CountDown = 0.0f;
	HitActors.Empty();
}


