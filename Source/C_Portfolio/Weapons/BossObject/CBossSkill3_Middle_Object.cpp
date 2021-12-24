#include "CBossSkill3_Middle_Object.h"
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
#include "CBossSkill1_Special_ThrowItem.h"

ACBossSkill3_Middle_Object::ACBossSkill3_Middle_Object()
{
	PrimaryActorTick.bCanEverTick = true;

	SphereCollision = CreateDefaultSubobject<USphereComponent>("SphereCollision");

	ParticleSystem = CreateDefaultSubobject<UParticleSystemComponent>("ParticleSystem");
	ParticleSystem->SetupAttachment(SphereCollision);

	AirbornePower = 1000.0f;
}

void ACBossSkill3_Middle_Object::BeginPlay()
{
	Super::BeginPlay();

	if (HitParticleClass != NULL)
	{
		FActorSpawnParameters params;
		params.Owner = GetOwner();
		FTransform transform;
		HitParticle = Cast<UParticleSystem>(GetWorld()->SpawnActor(HitParticleClass, &transform, params));
	}

}

void ACBossSkill3_Middle_Object::Tick(float DeltaTime)
{

}


void ACBossSkill3_Middle_Object::SetHitActors(AActor * InActor)
{
	ACharacter* character = Cast<ACharacter>(InActor);
	CheckNull(character);
	ExistHitActor(character);


}

void ACBossSkill3_Middle_Object::EndOverlap(AActor * InOtherActor)
{
	ACharacter* character = Cast<ACharacter>(InOtherActor);
	HitActors.Remove(character);
}

void ACBossSkill3_Middle_Object::Reset()
{
	HitActors.Empty();
}

void ACBossSkill3_Middle_Object::Action()
{
	CheckNull(OwnerSkill);

	for (ACharacter* character : HitActors)
	{
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
}


