#include "CAbility_Boss_Skill1.h"
#include "Global.h"
#include "GameFramework/Character.h"
//#include "Characters/CPlayer.h"
#include "Characters/CEnemy_Boss.h"

#include "Components/CSkillComponent.h"
#include "Components/CStateComponent.h"
#include "Components/ShapeComponent.h"
#include "Animation/AnimMontage.h"

#include "Weapons/CSkill.h"
#include "Weapons/BossObject/CBossSkill1_Middle_Object.h"
#include "Weapons/BossObject/CBossSkill1_Special_ThrowItem.h"

ACAbility_Boss_Skill1::ACAbility_Boss_Skill1()
{
	MiddleObjectForward = 50.0f;
}

void ACAbility_Boss_Skill1::BeginPlay()
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

void ACAbility_Boss_Skill1::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACAbility_Boss_Skill1::Behavior()
{
	OwnerBoss = Cast<ACEnemy_Boss>(OwnerCharacter);
	CheckNull(OwnerBoss);
	//CheckNull(player->GetEnemy());
	//CheckFalse(OwnerSkill->GetbReady());

	Super::Behavior();

	//if (bEnable == true)
	//{
	//	bExist = true;

	//	bEnable = false;

	//	return;
	//}
	CheckFalse(State->IsIdle());
	CheckFalse(OwnerSkill->GetbReady());
	State->SetSkillMode();

	CheckNull(MontageDatas);
	OwnerCharacter->GetCharacterMovement()->StopMovementImmediately();

	FRotator rot = CHelpers::YawRotator(UKismetMathLibrary::FindLookAtRotation(OwnerCharacter->GetActorLocation(), OwnerBoss->GetAggro()->GetActorLocation()));
	OwnerCharacter->SetActorRotation(rot);

	OwnerCharacter->PlayAnimMontage(MontageData->AnimMontage, MontageData->PlayRatio, MontageData->StartSection);
	CHelpers::SetCollisionPresetIgnoreOnlyPawn(OwnerCharacter->GetCapsuleComponent());
}
void ACAbility_Boss_Skill1::Begin_Behavior() //클릭하면
{
	OwnerSkill->SetbReady(false);
	OwnerSkill->SetSkillCool();

	//여기서 미리 스폰 (그 범위 보여주는 용도로..)
	FActorSpawnParameters params;
	params.Owner = this;

	FTransform transform = OwnerCharacter->GetTransform();
	FVector location = transform.GetLocation();
	location += OwnerCharacter->GetActorForwardVector() * MiddleObjectForward;
	transform.SetLocation(location);

	MiddleObject = GetWorld()->SpawnActor<ACBossSkill1_Middle_Object>(MiddleObjectClass, transform, params);
	MiddleObject->SetOwnerSkill(OwnerSkill);

}

void ACAbility_Boss_Skill1::Middle_Behavior()
{
	MiddleObject->Action();

	OwnerBoss->SetBladeColor(FLinearColor(1, 0, 0));

	MiddleObject->SetLifeSpan(0.5f);
}

void ACAbility_Boss_Skill1::Special_Behavior()
{
	FActorSpawnParameters params;
	params.Owner = this;

	FTransform transform = OwnerCharacter->GetTransform();
	
	ACBossSkill1_Special_ThrowItem* throwItem = GetWorld()->SpawnActor< ACBossSkill1_Special_ThrowItem>(ThrowItem, transform, params);
	throwItem->SetOwnerSkill(OwnerSkill);
	
}

void ACAbility_Boss_Skill1::End_Behavior()
{
	bStop = false;
	
	State->SetIdleMode();
	OwnerBoss->SetBladeColor(FLinearColor(1, 1, 1));
	CHelpers::SetCollisionPresetPawn(OwnerCharacter->GetCapsuleComponent());
}

void ACAbility_Boss_Skill1::OnColliders()
{
	TArray<UShapeComponent*> components;
	OwnerSkill->GetShapeComponent(components);

	for (UShapeComponent* component : components)
		component->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	//CLog::Log("OnColliders");
}

void ACAbility_Boss_Skill1::OffColliders()
{
	TArray<UShapeComponent*> components;
	OwnerSkill->GetShapeComponent(components);

	for (UShapeComponent* component : components)
		component->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	HitActors.Empty();
}

void ACAbility_Boss_Skill1::SkillPreview()
{

}

void ACAbility_Boss_Skill1::Cancel()
{
	CheckFalse(bStop);
	OffColliders();
	End_Behavior();
}


//float ACAbility_Boss_Skill1::GetPower()
//{
//	/*const FMontageData& data = MontageDatas->GetData()[Count];
//
//	return data.Power;*/
//}