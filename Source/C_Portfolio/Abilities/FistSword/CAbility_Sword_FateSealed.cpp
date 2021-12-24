#include "CAbility_Sword_FateSealed.h"
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

#include "Weapons/FistSword/CFateSealedObject.h"

void ACAbility_Sword_FateSealed::BeginPlay()
{
	Super::BeginPlay();

	InputType = SkillInputType::Position;
	PreviewType = SkillPreviewType::Arrow;

}

void ACAbility_Sword_FateSealed::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

void ACAbility_Sword_FateSealed::Behavior()
{
	Super::Behavior();
	CheckFalse(OwnerSkill->GetbReady());

	OwnerCharacter->GetCharacterMovement()->StopMovementImmediately();
	FRotator rot = CHelpers::YawRotator(UKismetMathLibrary::FindLookAtRotation(OwnerCharacter->GetActorLocation(), OwnerSkill->GetSkillComp()->GetTargetPosition()));
	OwnerCharacter->SetActorRotation(rot);

	State->SetSkillMode();

	OwnerCharacter->PlayAnimMontage(MontageData->AnimMontage, MontageData->PlayRatio, MontageData->StartSection);

}
void ACAbility_Sword_FateSealed::Begin_Behavior()
{
	//spawn
	OwnerSkill->SetbReady(false);
	OwnerSkill->SetSkillCool();

	FActorSpawnParameters params;
	params.Owner = this;

	FTransform transform = OwnerCharacter->GetActorTransform();

	Fate = GetWorld()->SpawnActor<ACFateSealedObject>(FateClass, transform, params);
	Fate->SetOwnerSkill(OwnerSkill);
}

void ACAbility_Sword_FateSealed::Special_Behavior()
{
	Fate->Action();
}

void ACAbility_Sword_FateSealed::End_Behavior()
{
	Fate->Destroy();
	State->SetIdleMode();
}

void ACAbility_Sword_FateSealed::SkillPreview()
{

}

void ACAbility_Sword_FateSealed::Cancel()
{
	bReady = false;
	bRangeCheck = false;
	State->SetIdleMode();
}
