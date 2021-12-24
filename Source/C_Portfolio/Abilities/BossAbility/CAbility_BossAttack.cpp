#include "CAbility_BossAttack.h"
#include "Global.h"
#include "GameFramework/Character.h"
//#include "Characters/CPlayer.h"
#include "Characters/CEnemy_Boss.h"
#include "Components/CSkillComponent.h"
#include "Components/CStateComponent.h"
#include "Components/ShapeComponent.h"
#include "Animation/AnimMontage.h"
#include "Weapons/CSkill.h"

void ACAbility_BossAttack::BeginPlay()
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

void ACAbility_BossAttack::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACAbility_BossAttack::Behavior()
{
	ACEnemy_Boss* boss = Cast<ACEnemy_Boss>(OwnerCharacter);
	CheckNull(boss);
	CheckNull(boss->GetAggro());
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
	CheckNull(MontageDatas);
	const FMontageData& data = MontageDatas->GetData()[0];

	FRotator rot = CHelpers::YawRotator(UKismetMathLibrary::FindLookAtRotation(OwnerCharacter->GetActorLocation(), boss->GetAggro()->GetActorLocation()));
	OwnerCharacter->SetActorRotation(rot);

	OwnerCharacter->PlayAnimMontage(data.AnimMontage, data.PlayRatio, data.StartSection);
	CHelpers::SetCollisionPresetIgnoreOnlyPawn(OwnerCharacter->GetCapsuleComponent());
	HitActors.Empty();
}
void ACAbility_BossAttack::Begin_Behavior() //클릭하면
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

void ACAbility_BossAttack::End_Behavior()
{
	const FMontageData& data = MontageDatas->GetData()[Count];
	OwnerCharacter->StopAnimMontage(data.AnimMontage);

	bStop = false;
	
	State->SetIdleMode();

	Count = 0;
	CHelpers::SetCollisionPresetPawn(OwnerCharacter->GetCapsuleComponent());
}

void ACAbility_BossAttack::OnColliders()
{
	TArray<UShapeComponent*> components;
	OwnerSkill->GetShapeComponent(components);

	for (UShapeComponent* component : components)
		component->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	//CLog::Log("OnColliders");
}

void ACAbility_BossAttack::OffColliders()
{
	TArray<UShapeComponent*> components;
	OwnerSkill->GetShapeComponent(components);

	for (UShapeComponent* component : components)
		component->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	HitActors.Empty();
}

void ACAbility_BossAttack::SkillPreview()
{

}

void ACAbility_BossAttack::Cancel()
{
	CheckFalse(bStop);
	OffColliders();
	End_Behavior();
}


//float ACAbility_BossAttack::GetPower()
//{
//	/*const FMontageData& data = MontageDatas->GetData()[Count];
//
//	return data.Power;*/
//}