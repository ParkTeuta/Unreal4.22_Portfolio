#include "CAbility_Projectile.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Characters/CPlayer.h"
#include "Components/CSkillComponent.h"
#include "Components/CStateComponent.h"
#include "Components/ShapeComponent.h"
#include "Animation/AnimMontage.h"
#include "Weapons/CSkill.h"
#include "Weapons/CThrowItem.h"

void ACAbility_Projectile::BeginPlay()
{
	Super::BeginPlay();

	TArray<UShapeComponent*> components;
	OwnerSkill->GetShapeComponent(components);
	InputType = SkillInputType::Enemy;

	Count = 1;

	for (UShapeComponent* component : components)
	{
		component->OnComponentBeginOverlap.AddDynamic(OwnerSkill, &ACSkill::OnComponentBeginOverlap);
		component->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	//CHelpers::GetClass<ACThrowItem>(&ThrowItem, "Blueprint'/Game/Skill/BP_CThrowItem.BP_CThrowItem_C'");

	//OwnerSkill->SetRange(500.0f);
}

void ACAbility_Projectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

void ACAbility_Projectile::Behavior()
{
	/*CheckNull(OwnerSkill->GetSkillComp()->GetEnemy());*/
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

	OwnerCharacter->PlayAnimMontage(MontageData->AnimMontage, MontageData->PlayRatio, MontageData->StartSection);
	HitActors.Empty();
	/*ACPlayer* player = Cast<ACPlayer>(OwnerCharacter);
	player->StopMove();
	FRotator rot = CHelpers::YawRotator(UKismetMathLibrary::FindLookAtRotation(player->GetActorLocation(), OwnerSkill->GetSkillComp()->GetEnemy()->GetActorLocation()));
	player->SetActorRotation(rot.Quaternion());*/

	//data.bCanMove ? Status->SetMoveMode() : Status->SetStopMode();
}
void ACAbility_Projectile::Begin_Behavior() //클릭하면
{
	/*CheckFalse(bReady);

	bReady = false;*/

	CLog::Log("Projectile");

	//spawn
	FActorSpawnParameters params;
	params.Owner = this;

	FTransform transform = OwnerCharacter->GetTransform();
	//spawn throwitem..
	ACThrowItem *temp = GetWorld()->SpawnActor<ACThrowItem>(ThrowItem, transform, params);
	temp->SetOwnerSkill(OwnerSkill);
}

void ACAbility_Projectile::End_Behavior()
{
	
	State->SetIdleMode();

}

void ACAbility_Projectile::SkillPreview()
{

}

void ACAbility_Projectile::Cancel()
{
	bReady = false;
	bRangeCheck = false;
}

