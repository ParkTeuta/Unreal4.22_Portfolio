#include "CBossSkill1_Middle_Object.h"
#include "Global.h"

#include "Weapons/CSkill.h"
#include "Abilities/CAbility.h"

#include "Particles/ParticleSystemComponent.h"
#include "Components/ShapeComponent.h"

ACBossSkill1_Middle_Object::ACBossSkill1_Middle_Object()
{
	ParticleSystem = CreateDefaultSubobject<UParticleSystemComponent>("ParticleSystem");
	ParticleSystem->SetupAttachment(RootComponent);
}


void ACBossSkill1_Middle_Object::BeginPlay()
{
	Super::BeginPlay();

	TArray<UShapeComponent*> components;
	GetShapeComponent(components);

	for (UShapeComponent* collision : components)
	{
		collision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	ParticleSystem->SetVisibility(false);
}

void ACBossSkill1_Middle_Object::SetHitActors(AActor * InActor)
{
	ACharacter* character = Cast<ACharacter>(InActor);
	CheckNull(character);
	ExistHitActor(character);
}
void ACBossSkill1_Middle_Object::EndOverlap(AActor * InOtherActor)
{
	ACharacter* character = Cast<ACharacter>(InOtherActor);
	HitActors.Remove(character);
}


void ACBossSkill1_Middle_Object::Action()
{
	ACharacter* ownerCharacter = OwnerSkill->GetOwnerCharacter();
	for (ACharacter* character : HitActors)
	{
		FDamageEvent e;
		Power = OwnerSkill->GetPower();
		character->TakeDamage(Power, e, ownerCharacter->GetController(), this);
	}

}

void ACBossSkill1_Middle_Object::GetShapeComponent(TArray<class UShapeComponent*>& OutComponents)
{
	TArray<UActorComponent *> components = GetComponentsByClass(UShapeComponent::StaticClass());

	for (UActorComponent* component : components)
	{
		UShapeComponent* temp = Cast<UShapeComponent>(component);

		if (!!temp)
			OutComponents.Add(temp);
	}
}
