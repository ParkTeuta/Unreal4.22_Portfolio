#include "CAbility_FarAttackCombo.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Characters/CPlayer.h"
#include "Components/CSkillComponent.h"
#include "Components/CStateComponent.h"
#include "Components/ShapeComponent.h"
#include "Animation/AnimMontage.h"
#include "Weapons/CSkill.h"
#include "Weapons/CThrowItem.h"

void ACAbility_FarAttackCombo::BeginPlay()
{
	Super::BeginPlay();

	TArray<UShapeComponent*> components;
	OwnerSkill->GetShapeComponent(components);
	InputType = SkillInputType::Enemy;

}

void ACAbility_FarAttackCombo::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACAbility_FarAttackCombo::Behavior()
{
	ACPlayer* player = Cast<ACPlayer>(OwnerCharacter);
	CheckNull(player);

	Super::Behavior();

	if (bEnable == true)
	{
		bExist = true;

		bEnable = false;

		return;
	}
	CheckFalse(State->IsIdle());

	State->SetAttackMode();
	CheckNull(MontageDatas);
	const FMontageData& data = MontageDatas->GetData()[0];
	OwnerCharacter->PlayAnimMontage(data.AnimMontage, data.PlayRatio, data.StartSection);

	HitActors.Empty();
}
void ACAbility_FarAttackCombo::Begin_Behavior() //클릭하면
{
	CheckFalse(bExist);

	bExist = false;


	Count++;
	const FMontageData& data = MontageDatas->GetData()[Count];
	OwnerCharacter->PlayAnimMontage(data.AnimMontage, data.PlayRatio, data.StartSection);
	//bStop = true;
}

void ACAbility_FarAttackCombo::Middle_Behavior()
{
	FActorSpawnParameters params;
	params.Owner = this;

	FTransform transform = OwnerCharacter->GetTransform();

	//spawn throwitem..
	ACThrowItem *temp = GetWorld()->SpawnActor<ACThrowItem>(ThrowItem, transform, params);
	temp->SetOwnerSkill(OwnerSkill);
}

void ACAbility_FarAttackCombo::End_Behavior()
{
	const FMontageData& data = MontageDatas->GetData()[Count];
	OwnerCharacter->StopAnimMontage(data.AnimMontage);

	bStop = false;
	
	State->SetIdleMode();

	Count = 0;
}

void ACAbility_FarAttackCombo::SkillPreview()
{

}

void ACAbility_FarAttackCombo::Cancel()
{
	CheckFalse(bStop);
	OffColliders();
	End_Behavior();
}
