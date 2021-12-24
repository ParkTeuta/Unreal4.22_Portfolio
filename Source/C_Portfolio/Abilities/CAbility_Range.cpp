#include "CAbility_Range.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Characters/CPlayer.h"
#include "Components/CSkillComponent.h"
#include "Components/CStateComponent.h"
#include "Components/ShapeComponent.h"
#include "Animation/AnimMontage.h"
#include "Weapons/CSkill.h"
#include "Engine/World.h"
#include "Engine/EngineTypes.h"

#include "Weapons/CRange.h"

void ACAbility_Range::BeginPlay()
{
	Super::BeginPlay();

	TArray<UShapeComponent*> components;
	OwnerSkill->GetShapeComponent(components);
	InputType = SkillInputType::Position;
	PreviewType = SkillPreviewType::Range;

	for (UShapeComponent* component : components)
	{
		component->OnComponentBeginOverlap.AddDynamic(OwnerSkill, &ACSkill::OnComponentBeginOverlap);
		component->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

}

void ACAbility_Range::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

void ACAbility_Range::Behavior()
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
	//CheckNull(OwnerSkill->GetSkillComp()->GetEnemy());

	State->SetSkillMode();


	OwnerCharacter->PlayAnimMontage(MontageData->AnimMontage, MontageData->PlayRatio, MontageData->StartSection);
	HitActors.Empty();
	/*ACPlayer* player = Cast<ACPlayer>(OwnerCharacter);
	player->StopMove();
	FRotator rot = CHelpers::YawRotator(UKismetMathLibrary::FindLookAtRotation(player->GetActorLocation(), OwnerSkill->GetSkillComp()->GetTargetPosition()));
	player->SetActorRotation(rot.Quaternion());*/
	//data.bCanMove ? Status->SetMoveMode() : Status->SetStopMode();
}
void ACAbility_Range::Begin_Behavior() //클릭하면
{
	/*CheckFalse(bReady);

	bReady = false;*/

	//spawn
	FActorSpawnParameters params;
	params.Owner = this;

	OwnerSkill->SetbReady(false);
	OwnerSkill->SetSkillCool();

	FTransform transform;
	transform.SetLocation(OwnerSkill->GetSkillComp()->GetTargetPosition());
	//spawn range..
	ACRange *range = GetWorld()->SpawnActor<ACRange>(Range, transform, params);
	range->SetOwnerSkill(OwnerSkill);
	//Check overlapMultiByChannel
	//TArray<FOverlapResult> overlap;
	//FVector pos = OwnerSkill->GetSkillComp()->GetTargetPosition();
	//FQuat rot = FQuat::Identity;
	//FCollisionShape collisionShape;
	//FCollisionQueryParams collisionQueryParams;
	//FCollisionResponseParams collisionResponseParams;

	//if (GetWorld()->OverlapMultiByChannel(overlap, pos, rot, ECollisionChannel::ECC_Pawn, collisionShape, collisionQueryParams, collisionResponseParams))
	//{
	//	FOverlapResult* result = overlap.GetData();

	//	//result->bBlockingHit
	//	CLog::Log(pos);
	//	if (!!Cast<ACharacter>(result->GetActor()))
	//		CLog::Log("Find it!");
	//	else
	//		CLog::Log("Not Founded..");

	//	//CLog::Log(result->Actor.IsValid());

	//}
	//else
	//	CLog::Log("Fail..");
}

void ACAbility_Range::End_Behavior()
{
	
	State->SetIdleMode();

}

void ACAbility_Range::SkillPreview()
{

}

void ACAbility_Range::Cancel()
{
	bReady = false;
	bRangeCheck = false;
	State->SetIdleMode();
}
