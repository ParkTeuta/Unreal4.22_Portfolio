#include "CAbility_Sword_CrossSword.h"
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

#include "Weapons/FistSword/CThrowItem_Sword_CrossSword.h"

void ACAbility_Sword_CrossSword::BeginPlay()
{
	Super::BeginPlay();

	InputType = SkillInputType::Immediately;
	PreviewType = SkillPreviewType::Arrow;

}

void ACAbility_Sword_CrossSword::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

void ACAbility_Sword_CrossSword::Behavior()
{
	Super::Behavior();
	CheckFalse(OwnerSkill->GetbReady());

	OwnerCharacter->GetCharacterMovement()->StopMovementImmediately();
	FRotator rot = CHelpers::YawRotator(UKismetMathLibrary::FindLookAtRotation(OwnerCharacter->GetActorLocation(), OwnerSkill->GetSkillComp()->GetTargetPosition()));
	OwnerCharacter->SetActorRotation(rot);

	State->SetSkillMode();

	OwnerCharacter->PlayAnimMontage(MontageData->AnimMontage, MontageData->PlayRatio, MontageData->StartSection);

	Ftemp = 1.0f;
}
void ACAbility_Sword_CrossSword::Begin_Behavior()
{
	//spawn
	OwnerSkill->SetbReady(false);
	OwnerSkill->SetSkillCool();

}

void ACAbility_Sword_CrossSword::End_Behavior()
{
	State->SetIdleMode();
}

void ACAbility_Sword_CrossSword::Middle_Behavior()
{
	//Sword_Trail_WhirlWind_End
	StartVec = OwnerCharacter->GetMesh()->GetSocketLocation(L"Sword_Trail_WhirlWind_End");


	FActorSpawnParameters params;
	params.Owner = this;

	FTransform transform;
	transform.SetLocation(OwnerCharacter->GetActorLocation());
	transform.SetRotation(OwnerCharacter->GetActorRotation().Quaternion());

	ACThrowItem_Sword_CrossSword* item = GetWorld()->SpawnActor<ACThrowItem_Sword_CrossSword>(ThrowItem, transform, params);
	item->SetBoxRotation(FRotator(0, 0, 40 * Ftemp));
	item->SetOwnerSkill(OwnerSkill);
	Ftemp *= -1.0f;
}

void ACAbility_Sword_CrossSword::SkillPreview()
{

}

void ACAbility_Sword_CrossSword::Cancel()
{
	bReady = false;
	bRangeCheck = false;
	State->SetIdleMode();
}
