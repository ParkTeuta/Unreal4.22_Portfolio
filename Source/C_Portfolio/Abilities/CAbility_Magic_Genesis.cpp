#include "CAbility_Magic_Genesis.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Components/CSkillComponent.h"
#include "Components/CStateComponent.h"
#include "Weapons/CSkill.h"
#include "Engine/World.h"
#include "Engine/EngineTypes.h"

#include "GameFramework/CharacterMovementComponent.h"

#include "Weapons/CGenesisAttack.h"

void ACAbility_Magic_Genesis::BeginPlay()
{
	Super::BeginPlay();

	TArray<UShapeComponent*> components;
	OwnerSkill->GetShapeComponent(components);
	InputType = SkillInputType::Immediately;
	PreviewType = SkillPreviewType::Range;


}

void ACAbility_Magic_Genesis::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

void ACAbility_Magic_Genesis::Behavior()
{
	Super::Behavior();

	CheckFalse(OwnerSkill->GetbReady());
	State->SetSkillMode();

	OwnerCharacter->GetCharacterMovement()->StopMovementImmediately();
	
	OwnerCharacter->PlayAnimMontage(MontageData->AnimMontage, MontageData->PlayRatio, MontageData->StartSection);

	FActorSpawnParameters params;
	params.Owner = this;


	FTransform transform;
	transform.SetLocation(OwnerCharacter->GetActorLocation());

	//spawn genes..
	genes = GetWorld()->SpawnActor<ACGenesisAttack>(Genesis, transform, params);
	genes->SetOwnerSkill(OwnerSkill);
}

void ACAbility_Magic_Genesis::Begin_Behavior() //클릭하면
{
	/*CheckFalse(bReady);

	bReady = false;*/
	OwnerSkill->SetbReady(false);
	OwnerSkill->SetSkillCool();
	if (genes != NULL)
		genes->Knockback();
}

void ACAbility_Magic_Genesis::End_Behavior()
{
	genes->Destroy();
	Cancel();
}

void ACAbility_Magic_Genesis::SkillPreview()
{

}

void ACAbility_Magic_Genesis::Cancel()
{
	bReady = false;
	bRangeCheck = false;
	genes->Destroy();

	State->SetIdleMode();
}
