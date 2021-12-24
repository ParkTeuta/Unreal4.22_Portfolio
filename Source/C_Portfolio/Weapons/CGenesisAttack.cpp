#include "CGenesisAttack.h"
#include "Global.h"
#include "CSkill.h"
#include "Components/ShapeComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Abilities/CAbility.h"

ACGenesisAttack::ACGenesisAttack()
{
	PrimaryActorTick.bCanEverTick = true;

	ParticleSystem = CreateDefaultSubobject<UParticleSystemComponent>("ParticleSystem");
	ParticleSystem->SetupAttachment(RootComponent);

	Montages = CreateDefaultSubobject<UCMontagesComponent>("Montages");

	KunckBackPower = 500.0f;
}

void ACGenesisAttack::GetShapeComponent(TArray<class UShapeComponent*>& OutComponents)
{
	TArray<UActorComponent *> components = GetComponentsByClass(UShapeComponent::StaticClass());

	for (UActorComponent* component : components)
	{
		UShapeComponent* temp = Cast<UShapeComponent>(component);

		if (!!temp)
			OutComponents.Add(temp);
	}
}


void ACGenesisAttack::BeginPlay()
{
	Super::BeginPlay();

	//SetLifeSpan(2.0f);

	if (Montages != NULL)
	{
		const FMontageData* data = Montages->GetSkillData();
		if (data != NULL)
			Radius = data->Radius;
	}
}

void ACGenesisAttack::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACGenesisAttack::SetHitActors(AActor * InActor)
{
	ACharacter* character = Cast<ACharacter>(InActor);
	CheckNull(character);
	ExistHitActor(character);
}

void ACGenesisAttack::EndOverlap(AActor * InOtherActor)
{
	ACharacter* character = Cast<ACharacter>(InOtherActor);
	HitActors.Remove(character);
}

void ACGenesisAttack::Knockback()
{
	CheckNull(GetOwnerSkill());
	for (AActor* actor : HitActors)
	{
		ACharacter* character = Cast<ACharacter>(actor);

		if (character == NULL || character == GetOwnerCharacter())
		{
			continue;
		}

		if (CanDameged(character) == false)
			continue;


		FVector characterVec = character->GetActorLocation();
		FVector thisVec = GetActorLocation();

		character->SetActorRotation(CHelpers::YawRotator(UKismetMathLibrary::FindLookAtRotation(characterVec, thisVec)));
		FRotator knockbackRot = CHelpers::YawRotator(UKismetMathLibrary::FindLookAtRotation(thisVec, characterVec));
		
		FVector velocity = knockbackRot.Vector() * KunckBackPower;
		character->LaunchCharacter(velocity, false, false);

		FDamageEvent e;
		float power = GetOwnerSkill()->GetPower();
		character->TakeDamage(power, e, GetOwnerCharacter()->GetController(), this);
		if (HitParticle != NULL)
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitParticle, character->GetActorLocation());
		}

		if(GetOwnerSkill()->GetHitSound()!=NULL)
			UGameplayStatics::SpawnSoundAttached(GetOwnerSkill()->GetHitSound(), character->GetRootComponent());

	}
}

