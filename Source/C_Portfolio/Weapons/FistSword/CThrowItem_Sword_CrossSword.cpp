#include "CThrowItem_Sword_CrossSword.h"
#include "Global.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Weapons/CSkill.h"

#include "Abilities/CAbility.h"
#include "Abilities/CAbility_Projectile.h"

ACThrowItem_Sword_CrossSword::ACThrowItem_Sword_CrossSword()
{
	PrimaryActorTick.bCanEverTick = true;

	SphereCollision = CreateDefaultSubobject<USphereComponent>("SphereCollision");
	SetRootComponent(SphereCollision);

	BoxCollision = CreateDefaultSubobject<UBoxComponent>("BoxCollision");
	BoxCollision->SetupAttachment(SphereCollision);

	ParticleSystem = CreateDefaultSubobject<UParticleSystemComponent>("ParticleSystem");
	ParticleSystem->SetupAttachment(BoxCollision);

	ProjectileComponent = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileComponent");
	ProjectileComponent->ProjectileGravityScale = 0.0f;

	ProjectileComponent->InitialSpeed = 600.0f;

	MovementDistance = 400.0f;

}

void ACThrowItem_Sword_CrossSword::SetBoxRotation(FRotator InRotator)
{
	BoxCollision->SetRelativeRotation(InRotator.Quaternion());
}

void ACThrowItem_Sword_CrossSword::BeginPlay()
{
	Super::BeginPlay();

	StartPos = GetActorLocation();
}

void ACThrowItem_Sword_CrossSword::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector CurrentPos = GetActorLocation();
	if (FVector::Distance(StartPos, CurrentPos) >= MovementDistance)
		Destroy();
	
}
