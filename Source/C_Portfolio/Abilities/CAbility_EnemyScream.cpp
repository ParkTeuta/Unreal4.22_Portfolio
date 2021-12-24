#include "CAbility_EnemyScream.h"
#include "Global.h"
#include "GameFramework/Character.h"
//#include "Characters/CPlayer.h"
#include "Characters/CEnemy.h"
#include "Components/CSkillComponent.h"
#include "Components/CStateComponent.h"
#include "Components/ShapeComponent.h"
#include "Animation/AnimMontage.h"
#include "Weapons/CSkill.h"
#include "AIController.h"

void ACAbility_EnemyScream::BeginPlay()
{
	Super::BeginPlay();

	TArray<UShapeComponent*> components;
	OwnerSkill->GetShapeComponent(components);
	InputType = SkillInputType::Enemy;

	//OwnerSkill->SetRange(200.0f);
}

void ACAbility_EnemyScream::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACAbility_EnemyScream::Behavior()
{
	ACEnemy* enemy = Cast<ACEnemy>(OwnerCharacter);
	CheckNull(enemy);

	Super::Behavior();

	CheckFalse(State->IsIdle());

	State->SetSkillMode();
	CheckNull(MontageDatas);

	OwnerCharacter->PlayAnimMontage(MontageData->AnimMontage, MontageData->PlayRatio, MontageData->StartSection);

	HitActors.Empty();
}
void ACAbility_EnemyScream::Begin_Behavior() //클릭하면
{
	BeginBehaviorAction();
}

void ACAbility_EnemyScream::BeginBehaviorAction_Implementation()
{
}

void ACAbility_EnemyScream::End_Behavior()
{
	State->SetIdleMode();
}

void ACAbility_EnemyScream::Cancel()
{
	CheckFalse(bStop);
	OffColliders();
	End_Behavior();
}


//float ACAbility_EnemyScream::GetPower()
//{
//	/*const FMontageData& data = MontageDatas->GetData()[Count];
//
//	return data.Power;*/
//}