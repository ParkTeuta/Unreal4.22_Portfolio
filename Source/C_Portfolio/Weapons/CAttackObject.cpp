#include "CAttackObject.h"
#include "Global.h"
#include "Particles/ParticleSystemComponent.h"
#include "CSkill.h"

#include "Abilities/CAbility.h"
#include "Characters/CNpc.h"
#include "Characters/CPlayer.h"

#include "Components/CStateComponent.h"

ACAttackObject::ACAttackObject()
{
	PrimaryActorTick.bCanEverTick = true;

}

void ACAttackObject::BeginOverlap(AActor * InOtherActor, UParticleSystem * InParticle, FVector & InLocation)
{
	CheckNull(OwnerSkill);

	ACAbility* ownerAbility = OwnerSkill->GetAbility();
	CheckNull(ownerAbility);

	float power = ownerAbility->GetPower();

	ACharacter* character = Cast<ACharacter>(InOtherActor);
	CheckNull(character);

	UCStateComponent* state = CHelpers::GetComponent<UCStateComponent>(character);
	CheckNull(state);
	CheckTrue(state->IsDead());
	CheckTrue(state->IsNone());

	ACharacter* ownerCharacter = OwnerSkill->GetOwnerCharacter();


	FDamageEvent e;
	if (!!ownerAbility)
	{
		if (InParticle != NULL)
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), InParticle, InOtherActor->GetActorLocation());
		}
		else if (HitParticle != NULL)
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitParticle, InOtherActor->GetActorLocation());
		}
		if (OwnerSkill->GetHitSound() != NULL)
			UGameplayStatics::SpawnSoundAttached(OwnerSkill->GetHitSound(), character->GetRootComponent());
		character->TakeDamage(power, e, ownerCharacter->GetController(), this);
	}
}

void ACAttackObject::BeginPlay()
{
	Super::BeginPlay();
	
}

void ACAttackObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

ACharacter * ACAttackObject::GetOwnerCharacter()
{
	CheckNullResult(OwnerSkill, NULL);
	return OwnerSkill->GetOwnerCharacter();
}

bool ACAttackObject::ExistHitActor(ACharacter * InCharacter)
{
	if (Count != 0 && HitActors.Num() == Count)
	{
		return true;
	}
	if (ExceptionCharacter(InCharacter))
	{
		return true;
	}
	for (ACharacter* character : HitActors)
	{
		if (character == InCharacter)
		{
			return true;
		}
	}
	HitActors.Add(InCharacter);
	return false;
}

bool ACAttackObject::ExceptionCharacter(ACharacter * InCharacter)
{
	if (InCharacter == GetOwnerCharacter())
		return true;
	ACNpc* npc = Cast<ACNpc>(InCharacter);
	CheckNullResult(npc, false);
	return true;
}

void ACAttackObject::SetOwnerSkill(ACSkill * InSkill)
{
	OwnerSkill = InSkill;
}

bool ACAttackObject::CanDameged(ACharacter * InCharacter)
{
	UCStateComponent* state = CHelpers::GetComponent<UCStateComponent>(InCharacter);
	CheckNullResult(state,false);
	CheckTrueResult(state->IsDead(), false);
	CheckTrueResult(state->IsNone(), false);

	return true;
}

