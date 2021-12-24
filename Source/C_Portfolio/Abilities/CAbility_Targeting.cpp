#include "CAbility_Targeting.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Characters/CPlayer.h"
#include "Components/CSkillComponent.h"
#include "Components/CStateComponent.h"
#include "Components/ShapeComponent.h"
#include "Animation/AnimMontage.h"
#include "Weapons/CSkill.h"

void ACAbility_Targeting::BeginPlay()
{
	Super::BeginPlay();

	TArray<UShapeComponent*> components;
	OwnerSkill->GetShapeComponent(components);
	InputType = SkillInputType::Enemy;

	for (UShapeComponent* component : components)
	{
		component->OnComponentBeginOverlap.AddDynamic(OwnerSkill, &ACSkill::OnComponentBeginOverlap);
		component->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}


	//OwnerSkill->SetRange(200.0f);
}

void ACAbility_Targeting::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//ACPlayer* player = Cast<ACPlayer>(OwnerCharacter);
	//if (bReady && bRangeCheck)
	//{
	//	ACPlayer* player = Cast<ACPlayer>(OwnerCharacter);
	//	CheckNull(player);
	//	if (player->GetDistanceTo(OwnerSkill->GetSkillComp()->GetEnemy()) > OwnerSkill->GetRange())
	//		player->MoveToTarget(OwnerSkill->GetSkillComp()->GetEnemy()->GetActorLocation());
	//	else
	//	{
	//		bRangeCheck = false;
	//		//attackMotion
	//		player->StopMove();
	//		player->SetActorRotation(UKismetMathLibrary::FindLookAtRotation(player->GetActorLocation(), OwnerSkill->GetSkillComp()->GetEnemy()->GetActorLocation()).Quaternion());
	//		Behavior();
	//	}
	//}
}

void ACAbility_Targeting::Behavior()
{
	CheckNull(OwnerSkill->GetSkillComp()->GetEnemy());
	if (!bReady)
	{
		bReady = true;
		bRangeCheck = true;
		return;
	}


	Super::Behavior();


	CheckFalse(OwnerSkill->GetbReady());
	CheckNull(OwnerSkill->GetSkillComp()->GetEnemy());

	State->SetSkillMode();
	OwnerSkill->SetbReady(false);
	OwnerSkill->SetSkillCool();
	CLog::Log(MontageData->PlayRatio);

	/*CheckFalse(MontageData->AnimMontage->HasValidSlotSetup());
	CLog::Log("AnimMontage Has Valid Slot Setup");*/
	OwnerCharacter->PlayAnimMontage(MontageData->AnimMontage, MontageData->PlayRatio, MontageData->StartSection);
	HitActors.Empty();
	//범위표시 이거아님

	//data.bCanMove ? Status->SetMoveMode() : Status->SetStopMode();
}
void ACAbility_Targeting::Begin_Behavior() //클릭하면
{
	CheckFalse(bReady);

	CLog::Log("Targeting");

	bReady = false;
}

void ACAbility_Targeting::End_Behavior()
{
	
	State->SetIdleMode();

}

void ACAbility_Targeting::OnColliders()
{
	TArray<UShapeComponent*> components;
	OwnerSkill->GetShapeComponent(components);

	for (UShapeComponent* component : components)
		component->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

}

void ACAbility_Targeting::OffColliders()
{
	TArray<UShapeComponent*> components;
	OwnerSkill->GetShapeComponent(components);

	for (UShapeComponent* component : components)
		component->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	HitActors.Empty();
}

void ACAbility_Targeting::SkillPreview()
{

}

void ACAbility_Targeting::Cancel()
{
	bReady = false;
	bRangeCheck = false;
	OffColliders();
}


//float ACAbility_Targeting::GetPower()
//{
//	/*const FMontageData& data = MontageDatas->GetData()[Count];
//
//	return data.Power;*/
//}