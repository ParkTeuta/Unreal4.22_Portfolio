#include "CThrowItem.h"
#include "Global.h"
#include "Components/SphereComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "CSkill.h"

#include "Abilities/CAbility.h"
#include "Abilities/CAbility_Projectile.h"

ACThrowItem::ACThrowItem()
{
	PrimaryActorTick.bCanEverTick = true;

	SphereCollision = CreateDefaultSubobject<USphereComponent>("SphereCollision");

	ParticleSystem = CreateDefaultSubobject<UParticleSystemComponent>("ParticleSystem");
	ParticleSystem->SetupAttachment(SphereCollision);

	ProjectileComponent = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileComponent");
	ProjectileComponent->ProjectileGravityScale = 0.0f;

	ProjectileComponent->InitialSpeed = 600.0f;

}

void ACThrowItem::BeginPlay()
{
	Super::BeginPlay();

	SetLifeSpan(2.0f);
	
}

void ACThrowItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//CLog::Log(GetActorLocation());
	if (target != NULL)
	{
		SetActorRotation(UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), target->GetActorLocation()).Quaternion());
	}
}


void ACThrowItem::BeginOverlapToTarget(AActor * InOtherActor, AActor * InTargetActor, UParticleSystem * InParticle, FVector & InLocation)
{
	CheckNull(OwnerSkill);

}
