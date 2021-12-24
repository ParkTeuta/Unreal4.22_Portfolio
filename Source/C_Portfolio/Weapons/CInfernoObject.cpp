#include "CInfernoObject.h"
#include "Global.h"

#include "CSkill.h"
#include "Abilities/CAbility.h"

#include "Particles/ParticleSystemComponent.h"
#include "Components/ShapeComponent.h"

ACInfernoObject::ACInfernoObject()
{

	ParticleSystem = CreateDefaultSubobject<UParticleSystemComponent>("ParticleSystem");
	ParticleSystem->SetupAttachment(RootComponent);
}

void ACInfernoObject::OnCollision()
{
	TArray<UShapeComponent*> components;
	GetShapeComponent(components);

	for (UShapeComponent* collision : components)
	{
		collision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}

	ParticleSystem->SetVisibility(true);
}

void ACInfernoObject::BeginPlay()
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

void ACInfernoObject::GetShapeComponent(TArray<class UShapeComponent*>& OutComponents)
{
	TArray<UActorComponent *> components = GetComponentsByClass(UShapeComponent::StaticClass());

	for (UActorComponent* component : components)
	{
		UShapeComponent* temp = Cast<UShapeComponent>(component);

		if (!!temp)
			OutComponents.Add(temp);
	}
}
