#include "CAbility_Dash_Magic.h"
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


void ACAbility_Dash_Magic::BeginPlay()
{
	Super::BeginPlay();

	TArray<UShapeComponent*> components;
	OwnerSkill->GetShapeComponent(components);
	InputType = SkillInputType::Position;

}

void ACAbility_Dash_Magic::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

void ACAbility_Dash_Magic::Behavior()
{

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
}
void ACAbility_Dash_Magic::Begin_Behavior() //클릭하면
{
	ACPlayer* player = Cast<ACPlayer>(OwnerCharacter);
	FRotator rot = CHelpers::YawRotator(UKismetMathLibrary::FindLookAtRotation(player->GetActorLocation(), OwnerSkill->GetSkillComp()->GetTargetPosition()));
	FVector location = player->GetActorLocation() + rot.Vector() * MoveDistance;
	
	Poseable();
	
	player->SetActorLocation(location);
}

void ACAbility_Dash_Magic::End_Behavior()
{
	
	State->SetIdleMode();

}

void ACAbility_Dash_Magic::SkillPreview()
{

}

void ACAbility_Dash_Magic::Cancel()
{
	bReady = false;
	bRangeCheck = false;
}


bool ACAbility_Dash_Magic::ExistHitActor(ACharacter * InCharacter)
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

void ACAbility_Dash_Magic::Poseable_Implementation()
{
}
