#include "CBossSkill1_Special_ThrowItem.h"
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

ACBossSkill1_Special_ThrowItem::ACBossSkill1_Special_ThrowItem()
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

void ACBossSkill1_Special_ThrowItem::BeginPlay()
{
	Super::BeginPlay();

	if (HitParticleClass != NULL)
	{
		FActorSpawnParameters params;
		params.Owner = GetOwner();
		FTransform transform;
		HitParticle = Cast<UParticleSystem>(GetWorld()->SpawnActor(HitParticleClass, &transform, params));
	}
	SetLifeSpan(LifeTime);
	
}

void ACBossSkill1_Special_ThrowItem::Tick(float DeltaTime)
{
		
}


void ACBossSkill1_Special_ThrowItem::SetHitActors(AActor * InActor)
{
	CheckNull(OwnerSkill);

	ACPlayer* character = Cast<ACPlayer>(InActor);
	CheckNull(character);

	if (ExistHitActor(character))
		return;

	ACharacter* ownerCharacter = OwnerSkill->GetOwnerCharacter();
	FDamageEvent e;
	Power = OwnerSkill->GetPower();
	character->TakeDamage(Power, e, ownerCharacter->GetController(), this);


	UCCrowdControlComponent* cc = CHelpers::GetComponent<UCCrowdControlComponent>(character);
	UCStateComponent* state = CHelpers::GetComponent<UCStateComponent>(character);
	if (cc != NULL && state != NULL)
	{
		cc->SetCCMode(ECrowdControl::Airborne);
		state->SetHittedMode();
	}
	character->GetMovementComponent()->StopMovementImmediately();

	FRotator characterRot = CHelpers::YawRotator(UKismetMathLibrary::FindLookAtRotation(character->GetActorLocation(), GetActorLocation()));
	character->SetActorRotation(characterRot.Quaternion());

	FRotator rot = -1 * characterRot;
	rot.Pitch += 60;

	character->GetCharacterMovement()->Launch(rot.Vector() * AirbornePower);

}

void ACBossSkill1_Special_ThrowItem::Reset()
{
	HitActors.Empty();
}


