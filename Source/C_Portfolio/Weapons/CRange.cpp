#include "CRange.h"
#include "Global.h"
#include "CSkill.h"
#include "Components/ShapeComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Abilities/CAbility.h"

ACRange::ACRange()
{
	PrimaryActorTick.bCanEverTick = true;

	ParticleSystem = CreateDefaultSubobject<UParticleSystemComponent>("ParticleSystem");
	ParticleSystem->SetupAttachment(RootComponent);

	Montages = CreateDefaultSubobject<UCMontagesComponent>("Montages");
}

void ACRange::GetShapeComponent(TArray<class UShapeComponent*>& OutComponents)
{
	TArray<UActorComponent *> components = GetComponentsByClass(UShapeComponent::StaticClass());

	for (UActorComponent* component : components)
	{
		UShapeComponent* temp = Cast<UShapeComponent>(component);

		if (!!temp)
			OutComponents.Add(temp);
	}
}


void ACRange::BeginPlay()
{
	Super::BeginPlay();

	SetLifeSpan(2.0f);

	if (Montages != NULL)
	{
		const FMontageData* data = Montages->GetSkillData();
		if (data != NULL)
			Radius = data->Radius;
	}
}

void ACRange::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
