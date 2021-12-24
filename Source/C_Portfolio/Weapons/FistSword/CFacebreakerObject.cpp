#include "CFacebreakerObject.h"
#include "Global.h"

#include "Weapons/CSkill.h"
#include "Abilities/CAbility.h"

#include "GameFramework/CharacterMovementComponent.h"

ACFacebreakerObject::ACFacebreakerObject()
{
	Power = 10.0f;
	GrabPower = 500.0f;
}

void ACFacebreakerObject::BeginPlay()
{
	Super::BeginPlay();

}

void ACFacebreakerObject::Tick(float DeltaTime)
{
}

void ACFacebreakerObject::EndOverlap(AActor * InOtherActor)
{
	ACharacter* character = Cast<ACharacter>(InOtherActor);
	HitActors.Remove(character);
}

void ACFacebreakerObject::SetHitActors(AActor * InActor)
{
	ACharacter* character = Cast<ACharacter>(InActor);
	CheckNull(character);
	ExistHitActor(character);
}

void ACFacebreakerObject::StopActors(AActor * InActor)
{
	ACharacter* character = Cast<ACharacter>(InActor);
	CheckNull(character);
	CheckFalse(HitActors.Find(character) != INDEX_NONE);
	character->GetMovementComponent()->StopMovementImmediately();
}

void ACFacebreakerObject::Action()
{
	CheckNull(GetOwnerSkill());

	for (ACharacter* character : HitActors)
	{
		if (character == GetOwnerCharacter())
			continue;

		if (CanDameged(character) == false)
			continue;

		FRotator rot = CHelpers::YawRotator(UKismetMathLibrary::FindLookAtRotation(character->GetActorLocation(), GetActorLocation()));
		character->SetActorRotation(rot.Quaternion());

		FVector power = rot.Vector() * GrabPower;
/*
		CLog::Log("Power is z :" + FString::SanitizeFloat(power.Z) + "   X : " + FString::SanitizeFloat(power.X) + "   Y : " + FString::SanitizeFloat(power.Y));
		
		CLog::Log(character->GetName());*/
		//character->LaunchCharacter(power, false, false);
		//GetOwnerCharacter()->LaunchCharacter(power, false, false);

		FDamageEvent e;
		character->TakeDamage(Power, e, GetOwnerCharacter()->GetController(), this);
		if (HitParticle != NULL)
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitParticle, character->GetActorLocation());
		}

		if (GetOwnerSkill()->GetHitSound() != NULL)
			UGameplayStatics::SpawnSoundAttached(GetOwnerSkill()->GetHitSound(), character->GetRootComponent());

		character->LaunchCharacter(power, false, false);
	}
}

