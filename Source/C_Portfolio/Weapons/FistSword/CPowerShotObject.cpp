#include "CPowerShotObject.h"
#include "Global.h"

#include "Weapons/CSkill.h"
#include "Abilities/CAbility.h"

#include "GameFramework/CharacterMovementComponent.h"

ACPowerShotObject::ACPowerShotObject()
{
	Power = 10.0f;

	SkillRangeDegree = 70.0f;
}

void ACPowerShotObject::BeginPlay()
{
	Super::BeginPlay();

}

void ACPowerShotObject::Tick(float DeltaTime)
{
}

void ACPowerShotObject::EndOverlap(AActor * InOtherActor)
{
	ACharacter* character = Cast<ACharacter>(InOtherActor);
	HitActors.Remove(character);
}

void ACPowerShotObject::SetHitActors(AActor * InActor)
{
	ACharacter* character = Cast<ACharacter>(InActor);
	CheckNull(character);
	
	ExistHitActor(character);
}


FVector ACPowerShotObject::GetRightLeftVector(bool IsRight)
{
	FVector dir = GetActorForwardVector();
	if (IsRight)
	{
		CHelpers::RotateVectorToDegree(dir, SkillRangeDegree / 2);
		return dir;
	}
	else
	{
		CHelpers::RotateVectorToDegree(dir, -SkillRangeDegree / 2);
		return dir;
	}
}

void ACPowerShotObject::Action()
{
	CheckNull(GetOwnerSkill());

	for (ACharacter* character : HitActors)
	{
		if (character == GetOwnerCharacter())
			continue;

		if (CanDameged(character) == false)
			continue;

		FVector forwardVec = GetActorForwardVector();
		//FVector forwardVec = GetOwnerCharacter()->GetActorForwardVector();
		//FTransform ownerTrans = GetOwnerCharacter()->GetTransform();
		//FRotator rightVec
		////ownerTrans.SetRotation(ownerTrans.GetRotation())
		FVector rightDir = forwardVec;
		CHelpers::RotateVectorToDegree(rightDir, SkillRangeDegree / 2);

		FVector targetDir = character->GetActorLocation() - GetOwnerCharacter()->GetActorLocation();
		targetDir.Normalize();
		float dotValue = Dot3(forwardVec, rightDir);

		Power = GetOwnerSkill()->GetPower();

		if (Dot3(forwardVec, targetDir) > dotValue)
		{
			FDamageEvent e;
			character->TakeDamage(Power, e, GetOwnerCharacter()->GetController(), this);
			if (HitParticle != NULL)
			{
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitParticle, character->GetActorLocation());
			}

			if (GetOwnerSkill()->GetHitSound() != NULL)
				UGameplayStatics::SpawnSoundAttached(GetOwnerSkill()->GetHitSound(), character->GetRootComponent());

			//CLog::Log(character->GetName() + " setHitActor Line 67");
		}
		//character->LaunchCharacter(power, false, false);

	}
}

