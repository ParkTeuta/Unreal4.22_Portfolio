#include "CEnemyThrowItem_Parabola.h"
#include "Global.h"
#include "Components/SphereComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "CSkill.h"
#include "Characters/CEnemy.h"
#include "Characters/CNpc.h"
#include "Abilities/CAbility.h"
#include "Abilities/CAbility_Projectile.h"

ACEnemyThrowItem_Parabola::ACEnemyThrowItem_Parabola()
{
	PrimaryActorTick.bCanEverTick = true;

	SphereCollision = CreateDefaultSubobject<USphereComponent>("SphereCollision");

	ParticleSystem = CreateDefaultSubobject<UParticleSystemComponent>("ParticleSystem");
	ParticleSystem->SetupAttachment(SphereCollision);

	ProjectileComponent = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileComponent");
}

void ACEnemyThrowItem_Parabola::BeginPlay()
{
	Super::BeginPlay();

	SetLifeSpan(2.0f);
	
}

void ACEnemyThrowItem_Parabola::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//CLog::Log(GetActorLocation());
	if (target != NULL)
	{
		SetActorRotation(UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), target->GetActorLocation()).Quaternion());
	}
}

bool ACEnemyThrowItem_Parabola::ExceptionCharacter(ACharacter * InCharacter)
{
	ACEnemy* enemy = Cast<ACEnemy>(InCharacter);
	CheckNullResult(enemy, false);
	ACNpc* npc = Cast<ACNpc>(InCharacter);
	CheckNullResult(npc, false);
	return true;
}


void ACEnemyThrowItem_Parabola::BeginOverlapToTarget(AActor * InOtherActor, AActor * InTargetActor, UParticleSystem * InParticle, FVector & InLocation)
{
	CheckNull(OwnerSkill);

}
