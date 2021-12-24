#include "CFateSealedObject.h"
#include "Global.h"

#include "Weapons/CSkill.h"
#include "Abilities/CAbility.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"


ACFateSealedObject::ACFateSealedObject()
{
	PrimaryActorTick.bCanEverTick = true;

	SphereCollision = CreateDefaultSubobject<USphereComponent>("SphereCollision");
	SetRootComponent(SphereCollision);

	BoxCollision = CreateDefaultSubobject<UBoxComponent>("BoxCollision");
	BoxCollision->SetupAttachment(SphereCollision);

	Power = 10.0f;
	GrabPower = 500.0f;
}

void ACFateSealedObject::BeginPlay()
{
	Super::BeginPlay();

	BoxCollision->SetRelativeLocation(FVector(Range, 0, 0));
	BoxCollision->SetBoxExtent(FVector(Range, 128, 32));

}

void ACFateSealedObject::Tick(float DeltaTime)
{
}

void ACFateSealedObject::EndOverlap(AActor * InOtherActor)
{
	ACharacter* character = Cast<ACharacter>(InOtherActor);
	HitActors.Remove(character);
}

void ACFateSealedObject::SetHitActors(AActor * InActor)
{
	ACharacter* character = Cast<ACharacter>(InActor);
	CheckNull(character);
	ExistHitActor(character);
}

void ACFateSealedObject::StopActors(AActor * InActor)
{
	ACharacter* character = Cast<ACharacter>(InActor);
	CheckNull(character);
	CheckFalse(HitActors.Find(character) != INDEX_NONE);
	character->GetMovementComponent()->StopMovementImmediately();
}

void ACFateSealedObject::Action()
{
	ACharacter* ownerCharacter = GetOwnerCharacter();
	if (HitActors.Num() == 0)
	{
		ownerCharacter->SetActorLocation(ownerCharacter->GetActorLocation() + (ownerCharacter->GetActorForwardVector() * Range));
		return;
	}
	ACharacter* farthest = NULL;
	float dist = 0;
	for (ACharacter* character : HitActors)
	{
		float dist2 = FVector::Distance(ownerCharacter->GetActorLocation(), character->GetActorLocation());
		if (dist2 >= dist)
		{
			dist = dist2;
			farthest = character;
		}
	}

	CheckNull(farthest);
	ownerCharacter->SetActorLocation(farthest->GetActorLocation() + (ownerCharacter->GetActorForwardVector() * 50.0f));
	EndAction();
}

void ACFateSealedObject::EndAction()
{
	CheckNull(GetOwnerSkill());

	for (ACharacter* character : HitActors)
	{
		if (character == GetOwnerCharacter())
			continue;

		if (CanDameged(character) == false)
			continue;

		FRotator rot = CHelpers::YawRotator(UKismetMathLibrary::FindLookAtRotation(character->GetActorLocation(), GetOwnerCharacter()->GetActorLocation()));
		character->SetActorRotation(rot.Quaternion());

		FVector power = rot.Vector() * GrabPower;
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

