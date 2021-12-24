#include "CAbility_Sword_SteelTempest.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Characters/CPlayer.h"
#include "Components/CSkillComponent.h"
#include "Components/CStateComponent.h"
#include "Components/ShapeComponent.h"
#include "Animation/AnimMontage.h"
#include "Weapons/CSkill.h"
#include "Engine/World.h"
#include "Engine/EngineTypes.h"

#include "GameFramework/CharacterMovementComponent.h"

#include "Weapons/FistSword/CThrowItem_Sword_Tempest.h"

void ACAbility_Sword_SteelTempest::BeginPlay()
{
	Super::BeginPlay();

	TArray<UShapeComponent*> components;
	OwnerSkill->GetShapeComponent(components);
	InputType = SkillInputType::Immediately;
	PreviewType = SkillPreviewType::Arrow;

	for (UShapeComponent* component : components)
	{
		component->OnComponentBeginOverlap.AddDynamic(OwnerSkill, &ACSkill::OnComponentBeginOverlap);
		component->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	Stack = 0;
	StackTime = 0.0f;
}

void ACAbility_Sword_SteelTempest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (Stack > 0)
	{
		StackTime += DeltaTime;
		if (StackTime >= StackDuration)
		{
			Stack = 0;
			StackTime = 0;
			ACPlayer* player = Cast<ACPlayer>(OwnerCharacter);
			if (player != NULL && OwnerSkill != NULL)
			{
				player->UpdateSkillSpriteInSkillButton(OwnerSkill->GetSkillButton(), MontageDatas->GetData()[0].Sprite);
			}
		}
	}
}

void ACAbility_Sword_SteelTempest::Behavior()
{
	Super::Behavior();
	CheckFalse(OwnerSkill->GetbReady());

	OwnerCharacter->GetCharacterMovement()->StopMovementImmediately();
	FRotator rot = CHelpers::YawRotator(UKismetMathLibrary::FindLookAtRotation(OwnerCharacter->GetActorLocation(), OwnerSkill->GetSkillComp()->GetTargetPosition()));
	OwnerCharacter->SetActorRotation(rot);

	State->SetSkillMode();

	if (Stack < 2)
	{
		const FMontageData& data = MontageDatas->GetData()[0];
		OwnerCharacter->PlayAnimMontage(data.AnimMontage, data.PlayRatio, data.StartSection);

		if (Stack == 1)
		{
			ACPlayer* player = Cast<ACPlayer>(OwnerCharacter);
			if (player != NULL && OwnerSkill != NULL)
			{
				player->UpdateSkillSpriteInSkillButton(OwnerSkill->GetSkillButton(), MontageDatas->GetData()[1].Sprite);
			}
		}
	}
	else
	{
		const FMontageData& data = MontageDatas->GetData()[1];
		OwnerCharacter->PlayAnimMontage(data.AnimMontage, data.PlayRatio, data.StartSection);
		OwnerSkill->SetSoundCount(1);
	}
}
void ACAbility_Sword_SteelTempest::Begin_Behavior()
{
	//spawn
	FActorSpawnParameters params;
	params.Owner = this;

	StackTime = 0.0f;

	OwnerSkill->SetbReady(false);
	OwnerSkill->SetSkillCool();


	if (Stack >= 2)
	{
		FTransform transform;
		transform = OwnerCharacter->GetTransform();
		//spawn range..
		class ACThrowItem_Sword_Tempest * tempest = GetWorld()->SpawnActor<ACThrowItem_Sword_Tempest>(Tempest, transform, params);
		tempest->SetOwnerSkill(OwnerSkill);

		ACPlayer* player = Cast<ACPlayer>(OwnerCharacter);
		if (player != NULL && OwnerSkill != NULL)
		{
			player->UpdateSkillSpriteInSkillButton(OwnerSkill->GetSkillButton(), MontageDatas->GetData()[0].Sprite);
		}
	}
}

void ACAbility_Sword_SteelTempest::End_Behavior()
{
	//Inferno->Destroy();
	Stack++;

	if (Stack >= 3)
	{
		Stack = 0;
		OwnerSkill->SetSoundCount(0);
	}
	State->SetIdleMode();
}


void ACAbility_Sword_SteelTempest::OnColliders()
{
	TArray<UShapeComponent*> components;
	OwnerSkill->GetShapeComponent(components);

	for (UShapeComponent* component : components)
		component->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

void ACAbility_Sword_SteelTempest::OffColliders()
{
	TArray<UShapeComponent*> components;
	OwnerSkill->GetShapeComponent(components);

	for (UShapeComponent* component : components)
		component->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	HitActors.Empty();
}

void ACAbility_Sword_SteelTempest::SkillPreview()
{

}

void ACAbility_Sword_SteelTempest::Cancel()
{
	bReady = false;
	bRangeCheck = false;
	State->SetIdleMode();
}
