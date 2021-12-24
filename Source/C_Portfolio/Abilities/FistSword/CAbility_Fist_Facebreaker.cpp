#include "CAbility_Fist_Facebreaker.h"
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

#include "GameFramework/CharacterMovementComponent.h"

#include "Weapons/FistSword/CFacebreakerObject.h"

void ACAbility_Fist_Facebreaker::BeginPlay()
{
	Super::BeginPlay();

	TArray<UShapeComponent*> components;
	OwnerSkill->GetShapeComponent(components);
	InputType = SkillInputType::Immediately;
	PreviewType = SkillPreviewType::Range;

	for (UShapeComponent* component : components)
	{
		component->OnComponentBeginOverlap.AddDynamic(OwnerSkill, &ACSkill::OnComponentBeginOverlap);
		component->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

}

void ACAbility_Fist_Facebreaker::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

void ACAbility_Fist_Facebreaker::Behavior()
{
	Super::Behavior();
	CheckFalse(OwnerSkill->GetbReady());

	OwnerCharacter->GetCharacterMovement()->StopMovementImmediately();
	FRotator rot = CHelpers::YawRotator(UKismetMathLibrary::FindLookAtRotation(OwnerCharacter->GetActorLocation(), OwnerSkill->GetSkillComp()->GetTargetPosition()));
	rot.Yaw -= 90;
	OwnerCharacter->SetActorRotation(rot);

	State->SetSkillMode();

	OwnerCharacter->PlayAnimMontage(MontageData->AnimMontage, MontageData->PlayRatio, MontageData->StartSection);
}
void ACAbility_Fist_Facebreaker::Begin_Behavior()
{
	//spawn
	FActorSpawnParameters params;
	params.Owner = this;

	OwnerSkill->SetbReady(false);
	OwnerSkill->SetSkillCool();

	FTransform transform;
	transform = OwnerCharacter->GetTransform();
	//spawn range..
	
	Facebreaker = GetWorld()->SpawnActor<ACFacebreakerObject>(FacebreakerClass, transform, params);
	Facebreaker->SetOwnerSkill(OwnerSkill);

	Facebreaker->Action();
}
//
//void ACAbility_Fist_Facebreaker::Special_Behavior()
//{
//	CheckNull(Inferno);
//
//	Inferno->OnCollision();
//}

void ACAbility_Fist_Facebreaker::End_Behavior()
{
	//Inferno->Destroy();
	
	State->SetIdleMode();
	Facebreaker->Destroy();
}

void ACAbility_Fist_Facebreaker::SkillPreview()
{

}

void ACAbility_Fist_Facebreaker::Cancel()
{
	bReady = false;
	bRangeCheck = false;
	State->SetIdleMode();
}
