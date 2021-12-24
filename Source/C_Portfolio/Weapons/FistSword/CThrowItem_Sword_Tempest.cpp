#include "CThrowItem_Sword_Tempest.h"
#include "Global.h"
#include "Components/SphereComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Weapons/CSkill.h"

#include "Abilities/CAbility.h"
#include "Abilities/CAbility_Projectile.h"

ACThrowItem_Sword_Tempest::ACThrowItem_Sword_Tempest()
{
	PrimaryActorTick.bCanEverTick = true;

	SphereCollision = CreateDefaultSubobject<USphereComponent>("SphereCollision");

	ParticleSystem = CreateDefaultSubobject<UParticleSystemComponent>("ParticleSystem");
	ParticleSystem->SetupAttachment(SphereCollision);

	ProjectileComponent = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileComponent");
	ProjectileComponent->ProjectileGravityScale = 0.0f;

	ProjectileComponent->InitialSpeed = 600.0f;

	MovementDistance = 400.0f;

}

void ACThrowItem_Sword_Tempest::BeginPlay()
{
	Super::BeginPlay();

	StartPos = GetActorLocation();
}

void ACThrowItem_Sword_Tempest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector CurrentPos = GetActorLocation();
	if (FVector::Distance(StartPos, CurrentPos) >= MovementDistance)
		Destroy();
	
}
