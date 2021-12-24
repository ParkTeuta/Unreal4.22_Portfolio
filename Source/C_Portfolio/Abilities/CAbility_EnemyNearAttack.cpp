#include "CAbility_EnemyNearAttack.h"
#include "Global.h"
#include "GameFramework/Character.h"
//#include "Characters/CPlayer.h"
#include "Characters/CEnemy.h"
#include "Components/CSkillComponent.h"
#include "Components/CStateComponent.h"
#include "Components/ShapeComponent.h"
#include "Animation/AnimMontage.h"
#include "Weapons/CSkill.h"

void ACAbility_EnemyNearAttack::BeginPlay()
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

void ACAbility_EnemyNearAttack::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACAbility_EnemyNearAttack::Behavior()
{
	ACEnemy* enemy = Cast<ACEnemy>(OwnerCharacter);
	CheckNull(enemy);
	//CheckNull(player->GetEnemy());
	//CheckFalse(OwnerSkill->GetbReady());

	Super::Behavior();

	if (bEnable == true)
	{
		bExist = true;

		bEnable = false;

		return;
	}
	CheckFalse(State->IsIdle());

	State->SetAttackMode();

	CheckNull(enemy->GetAggro());
	enemy->GetMovementComponent()->StopMovementImmediately();

	FRotator rot = CHelpers::YawRotator(UKismetMathLibrary::FindLookAtRotation(enemy->GetActorLocation(), enemy->GetAggro()->GetActorLocation()));
	enemy->SetActorRotation(rot.Quaternion());

	CheckNull(MontageDatas);
	const FMontageData& data = MontageDatas->GetData()[0];
	OwnerCharacter->PlayAnimMontage(data.AnimMontage, data.PlayRatio, data.StartSection);

	HitActors.Empty();
}
void ACAbility_EnemyNearAttack::Begin_Behavior() //클릭하면
{
	CheckFalse(bExist);

	bExist = false;


	Count++;
	const FMontageData& data = MontageDatas->GetData()[Count];
	OwnerCharacter->PlayAnimMontage(data.AnimMontage, data.PlayRatio, data.StartSection);
	//OwnerSkill->SetSkillCool();
	//OwnerSkill->SetbReady(false);
	bStop = true;
}

void ACAbility_EnemyNearAttack::End_Behavior()
{
	const FMontageData& data = MontageDatas->GetData()[Count];
	OwnerCharacter->StopAnimMontage(data.AnimMontage);

	bStop = false;
	
	State->SetIdleMode();

	Count = 0;
}

void ACAbility_EnemyNearAttack::OnColliders()
{
	TArray<UShapeComponent*> components;
	OwnerSkill->GetShapeComponent(components);

	for (UShapeComponent* component : components)
	{
		component->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	}
	//CLog::Log("OnColliders");
}

void ACAbility_EnemyNearAttack::OffColliders()
{
	TArray<UShapeComponent*> components;
	OwnerSkill->GetShapeComponent(components);

	for (UShapeComponent* component : components)
		component->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	HitActors.Empty();
}

void ACAbility_EnemyNearAttack::SkillPreview()
{

}

void ACAbility_EnemyNearAttack::Cancel()
{
	CheckFalse(bStop);
	OffColliders();
	End_Behavior();
}


//float ACAbility_EnemyNearAttack::GetPower()
//{
//	/*const FMontageData& data = MontageDatas->GetData()[Count];
//
//	return data.Power;*/
//}