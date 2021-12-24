#include "CAbility.h"
#include "Global.h"
#include "Weapons/CSkill.h"
#include "Characters/CPlayer.h"
#include "Characters/CEnemy.h"

ACAbility::ACAbility()
{
	PrimaryActorTick.bCanEverTick = true;

}

void ACAbility::BeginPlay()
{

	OwnerSkill = Cast<ACSkill>(GetOwner());
	OwnerCharacter = OwnerSkill->GetOwnerCharacter();
	State = OwnerSkill->GetState();
	Status = OwnerSkill->GetStatus();

	Super::BeginPlay();
}

void ACAbility::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool ACAbility::ExceptionCharacter(ACharacter * InCharacter)
{
	return false;
}

ACPlayer * ACAbility::GetOwnerPlayer()
{
	return Cast<ACPlayer>(OwnerCharacter);
}

void ACAbility::PushForward()
{
	OwnerCharacter->LaunchCharacter(OwnerCharacter->GetActorForwardVector() * PushForwardPower, false, false);
}

void ACAbility::Cancel()
{
	State->SetIdleMode();
}

bool ACAbility::ExistHitActor(ACharacter * InCharacter)
{

	if (Count != 0 && HitActors.Num() == Count)
		return true;
	if (ExceptionCharacter(InCharacter))
	{
		return true;
	}
	for (ACharacter* character : HitActors)
	{
		if (character == InCharacter)
			return true;
	}
	HitActors.Add(InCharacter);
	return false;
}

