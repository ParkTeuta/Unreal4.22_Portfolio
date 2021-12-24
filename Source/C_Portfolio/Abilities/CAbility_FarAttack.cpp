#include "CAbility_FarAttack.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Characters/CPlayer.h"
#include "Components/CSkillComponent.h"
#include "Components/CStateComponent.h"
#include "Components/ShapeComponent.h"
#include "Animation/AnimMontage.h"
#include "Weapons/CSkill.h"
#include "Weapons/CThrowItem.h"

void ACAbility_FarAttack::BeginPlay()
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

	//CHelpers::GetClass<ACThrowItem>(&ThrowItem, "Blueprint'/Game/Skill/BP_CThrowItem.BP_CThrowItem_C'");

	//OwnerSkill->SetRange(500.0f);
}

void ACAbility_FarAttack::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

void ACAbility_FarAttack::Behavior()
{
	/*CheckNull(OwnerSkill->GetSkillComp()->GetEnemy());*/
	/*if (!bReady)
	{
		bReady = true;
		bRangeCheck = true;
		return;
	}
*/

	Super::Behavior();

	
	CheckFalse(OwnerSkill->GetbReady());
	ACPlayer* player = Cast<ACPlayer>(OwnerCharacter);
	CheckNull(player);
	//CheckNull(player->GetEnemy());
	//CheckNull(OwnerSkill->GetSkillComp()->GetEnemy());

	State->SetAttackMode();
	OwnerSkill->SetbReady(false);
	OwnerSkill->SetSkillCool();

	OwnerCharacter->PlayAnimMontage(MontageData->AnimMontage, MontageData->PlayRatio, MontageData->StartSection);
	HitActors.Empty();
	player->StopMove();
	//player->SetActorRotation(UKismetMathLibrary::FindLookAtRotation(player->GetActorLocation(), player->GetEnemy()->GetActorLocation()).Quaternion());
	//data.bCanMove ? Status->SetMoveMode() : Status->SetStopMode();
}
void ACAbility_FarAttack::Begin_Behavior() //클릭하면
{
	/*CheckFalse(bReady);

	bReady = false;*/


	//spawn
	FActorSpawnParameters params;
	params.Owner = this;

	FTransform transform = OwnerCharacter->GetTransform();
	//spawn throwitem..
	ACThrowItem *temp = GetWorld()->SpawnActor<ACThrowItem>(ThrowItem, transform, params);
	temp->SetOwnerSkill(OwnerSkill);
	//ACPlayer* player = Cast<ACPlayer>(OwnerCharacter);
	//temp->SetTarget(player->GetEnemy());
}

void ACAbility_FarAttack::End_Behavior()
{
	
	State->SetIdleMode();

}

void ACAbility_FarAttack::SkillPreview()
{

}

void ACAbility_FarAttack::Cancel()
{
	bReady = false;
	bRangeCheck = false;

	State->SetIdleMode();
}
