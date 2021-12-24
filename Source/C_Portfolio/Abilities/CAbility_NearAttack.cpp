#include "CAbility_NearAttack.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Characters/CPlayer.h"
#include "Components/CSkillComponent.h"
#include "Components/CStateComponent.h"
#include "Components/ShapeComponent.h"
#include "Animation/AnimMontage.h"
#include "Weapons/CSkill.h"

void ACAbility_NearAttack::BeginPlay()
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

void ACAbility_NearAttack::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACAbility_NearAttack::Behavior()
{
	ACPlayer* player = Cast<ACPlayer>(OwnerCharacter);
	CheckNull(player);
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
	OwnerCharacter->PlayAnimMontage(data.AnimMontage, data.PlayRatio, data.StartSection);

	HitActors.Empty();
}
void ACAbility_NearAttack::Begin_Behavior() //클릭하면
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

void ACAbility_NearAttack::End_Behavior()
{
	const FMontageData& data = MontageDatas->GetData()[Count];
	OwnerCharacter->StopAnimMontage(data.AnimMontage);

	bStop = false;
	
	State->SetIdleMode();

	Count = 0;
}

void ACAbility_NearAttack::OnColliders()
{
	TArray<UShapeComponent*> components;
	OwnerSkill->GetShapeComponent(components);
	if (components.Num() != 0)
	{
		for (UShapeComponent* component : components)
			component->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	}
	else
	{
		for (UShapeComponent* component : OwnerSkill->GetClassColliders())
		{
			component->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		}
	}
	//CLog::Log("OnColliders");
}

void ACAbility_NearAttack::OffColliders()
{
	TArray<UShapeComponent*> components;
	OwnerSkill->GetShapeComponent(components);

	if (components.Num() != 0)
	{
		for (UShapeComponent* component : components)
			component->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	else
	{
		for(UShapeComponent* component : OwnerSkill->GetClassColliders())
			component->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	HitActors.Empty();
}

void ACAbility_NearAttack::SkillPreview()
{

}

void ACAbility_NearAttack::Cancel()
{
	CheckFalse(bStop);
	OffColliders();
	End_Behavior();
}


//float ACAbility_NearAttack::GetPower()
//{
//	/*const FMontageData& data = MontageDatas->GetData()[Count];
//
//	return data.Power;*/
//}