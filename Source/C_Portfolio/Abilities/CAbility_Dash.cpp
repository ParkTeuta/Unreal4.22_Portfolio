#include "CAbility_Dash.h"
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


void ACAbility_Dash::BeginPlay()
{
	Super::BeginPlay();

	TArray<UShapeComponent*> components;
	OwnerSkill->GetShapeComponent(components);
	InputType = SkillInputType::Position;

}

void ACAbility_Dash::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

void ACAbility_Dash::Behavior()
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
	//CheckNull(OwnerSkill->GetSkillComp()->GetEnemy());

	State->SetSkillMode();
	OwnerSkill->SetbReady(false);
	OwnerSkill->SetSkillCool();

	OwnerCharacter->PlayAnimMontage(MontageData->AnimMontage, MontageData->PlayRatio, MontageData->StartSection);
	HitActors.Empty();
	ACPlayer* player = Cast<ACPlayer>(OwnerCharacter);
	player->StopMove();
	FRotator rot = CHelpers::YawRotator(UKismetMathLibrary::FindLookAtRotation(player->GetActorLocation(), OwnerSkill->GetSkillComp()->GetTargetPosition()));
	player->SetActorRotation(rot.Quaternion());
	//data.bCanMove ? Status->SetMoveMode() : Status->SetStopMode();
}
void ACAbility_Dash::Begin_Behavior() //클릭하면
{
	/*CheckFalse(bReady);

	bReady = false;*/
	CHelpers::SetCollisionPresetIgnoreOnlyPawn(OwnerCharacter->GetCapsuleComponent());

	ACPlayer* player = Cast<ACPlayer>(OwnerCharacter);
	FRotator rot = CHelpers::YawRotator(UKismetMathLibrary::FindLookAtRotation(player->GetActorLocation(), OwnerSkill->GetSkillComp()->GetTargetPosition()));
	FVector velocity = rot.Vector() * DashPower;
	player->LaunchCharacter(velocity, false, false);

	Poseable();
}

void ACAbility_Dash::End_Behavior()
{
	
	State->SetIdleMode();
	CHelpers::SetCollisionPresetPawn(OwnerCharacter->GetCapsuleComponent());
}

void ACAbility_Dash::SkillPreview()
{

}

void ACAbility_Dash::Cancel()
{
	bReady = false;
	bRangeCheck = false;
}


bool ACAbility_Dash::ExistHitActor(ACharacter * InCharacter)
{
	if (HitActors.Num() == Count)
		return true;
	for (ACharacter* character : HitActors)
	{
		if (character == InCharacter)
			return true;
	}
	HitActors.Add(InCharacter);
	return false;
}

void ACAbility_Dash::Poseable_Implementation()
{
}
