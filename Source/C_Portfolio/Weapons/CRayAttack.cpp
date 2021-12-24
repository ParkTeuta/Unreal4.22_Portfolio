#include "CRayAttack.h"
#include "Global.h"

#include "CSkill.h"

#include "Abilities/CAbility.h"

#include "Sound/SoundBase.h"


ACRayAttack::ACRayAttack()
{
	TickTime = 0.0f;
	CountDown = 0.0f;
	Power = 10.0f;
}

void ACRayAttack::Tick(float DeltaTime)
{
	CheckNull(GetOwnerSkill());

	if (HitActors.Num() > 0)
	{
		if (TickTime <= CountDown)
		{
			ACharacter* ownerCharacter = OwnerSkill->GetOwnerCharacter();
			//Power = GetOwnerSkill()->GetPower();
			for (ACharacter* character : HitActors)
			{
				if (CanDameged(character) == false)
					continue;

				FDamageEvent e;
				Power = OwnerSkill->GetPower();
				character->TakeDamage(Power, e, ownerCharacter->GetController(), this);
				if (HitParticle != NULL)
				{
					UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitParticle, character->GetActorLocation());
				}

				if (GetOwnerSkill()->GetHitSound() != NULL)
					UGameplayStatics::SpawnSoundAttached(GetOwnerSkill()->GetHitSound(), character->GetRootComponent());
			}
			CountDown = 0;

			if (PlaneSound != NULL)
				UGameplayStatics::SpawnSoundAtLocation(GetWorld(), PlaneSound, GetActorLocation(), FRotator::ZeroRotator, Volume);

		}
		else
			CountDown += DeltaTime;
	}
}

void ACRayAttack::BeginOverlap(AActor * InOtherActor, UParticleSystem * InParticle, FVector & InLocation)
{
	CheckNull(OwnerSkill);
	ACAbility* ownerAbility = OwnerSkill->GetAbility();
	CheckNull(ownerAbility);
	
	ACharacter* character = Cast<ACharacter>(InOtherActor);
	CheckNull(character);

	ExistHitActor(character);
}


void ACRayAttack::EndOverlap(AActor * InOtherActor)
{
	ACharacter* character = Cast<ACharacter>(InOtherActor);
	HitActors.Remove(character);
}

void ACRayAttack::SetHitActors(AActor * InActor)
{
	ACharacter* character = Cast<ACharacter>(InActor);
	CheckNull(character);
	ExistHitActor(character);
}

void ACRayAttack::Reset()
{
	CountDown = 0.0f;
	HitActors.Empty();
}

void ACRayAttack::SetPower(float InPower)
{
	Power = InPower; 
}
