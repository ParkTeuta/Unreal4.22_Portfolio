#include "CAbility_EnemyFarAttack.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Characters/CPlayer.h"
#include "Characters/CEnemy.h"
#include "Components/CSkillComponent.h"
#include "Components/CStateComponent.h"
#include "Components/ShapeComponent.h"
#include "Animation/AnimMontage.h"
#include "Weapons/CSkill.h"
#include "Weapons/CEnemyThrowItem.h"
#include "CAbility_Enemy_HerakliosAttack.h"


void ACAbility_EnemyFarAttack::BeginPlay()
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

void ACAbility_EnemyFarAttack::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

void ACAbility_EnemyFarAttack::Behavior()
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

	
	//CheckFalse(OwnerSkill->GetbReady());
	ACEnemy* enemy = Cast<ACEnemy>(OwnerCharacter);
	CheckNull(enemy);
	CheckNull(enemy->GetAggro());
	//CheckNull(player->GetEnemy());
	//CheckNull(OwnerSkill->GetSkillComp()->GetEnemy());

	State->SetAttackMode();
	OwnerSkill->SetbReady(false);
	OwnerSkill->SetSkillCool();

	enemy->GetMovementComponent()->StopMovementImmediately();

	FRotator rot = CHelpers::YawRotator(UKismetMathLibrary::FindLookAtRotation(enemy->GetActorLocation(), enemy->GetAggro()->GetActorLocation()));
	enemy->SetActorRotation(rot.Quaternion());


	OwnerCharacter->PlayAnimMontage(MontageData->AnimMontage, MontageData->PlayRatio, MontageData->StartSection);
	HitActors.Empty();
	//player->StopMove();
	//player->SetActorRotation(UKismetMathLibrary::FindLookAtRotation(player->GetActorLocation(), player->GetEnemy()->GetActorLocation()).Quaternion());
	//data.bCanMove ? Status->SetMoveMode() : Status->SetStopMode();
}
void ACAbility_EnemyFarAttack::Begin_Behavior() //클릭하면
{
	/*CheckFalse(bReady);

	bReady = false;*/


	//spawn
	FActorSpawnParameters params;
	params.Owner = this;

	FTransform transform = OwnerCharacter->GetTransform();
	//spawn throwitem..
	ACEnemyThrowItem *temp = GetWorld()->SpawnActor<ACEnemyThrowItem>(ThrowItem, transform, params);
	temp->SetOwnerSkill(OwnerSkill);
	//ACPlayer* player = Cast<ACPlayer>(OwnerCharacter);
	//temp->SetTarget(player->GetEnemy());
}

void ACAbility_EnemyFarAttack::End_Behavior()
{
	State->SetIdleMode();

}

void ACAbility_EnemyFarAttack::SkillPreview()
{

}

void ACAbility_EnemyFarAttack::Cancel()
{
	bReady = false;
	bRangeCheck = false;
}

bool ACAbility_EnemyFarAttack::ExceptionCharacter(ACharacter * InCharacter)
{
	ACEnemy* enemy = Cast<ACEnemy>(InCharacter);
	CLog::Log("Line 109");
	CheckNullResult(enemy, false);
	CLog::Log("Line 111");
	return true;
}
