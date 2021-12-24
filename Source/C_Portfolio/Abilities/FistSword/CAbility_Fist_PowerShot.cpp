#include "CAbility_Fist_PowerShot.h"
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

#include "Weapons/FistSword/CPowerShotObject.h"

void ACAbility_Fist_PowerShot::BeginPlay()
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

void ACAbility_Fist_PowerShot::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

void ACAbility_Fist_PowerShot::Behavior()
{
	Super::Behavior();
	CheckFalse(OwnerSkill->GetbReady());

	OwnerCharacter->GetCharacterMovement()->StopMovementImmediately();
	FRotator rot = CHelpers::YawRotator(UKismetMathLibrary::FindLookAtRotation(OwnerCharacter->GetActorLocation(), OwnerSkill->GetSkillComp()->GetTargetPosition()));
	OwnerCharacter->SetActorRotation(rot);

	State->SetSkillMode();

	OwnerCharacter->PlayAnimMontage(MontageData->AnimMontage, MontageData->PlayRatio, MontageData->StartSection);
}
void ACAbility_Fist_PowerShot::Begin_Behavior()
{
	//spawn
	FActorSpawnParameters params;
	params.Owner = this;

	OwnerSkill->SetbReady(false);
	OwnerSkill->SetSkillCool();

	FTransform transform;
	transform = OwnerCharacter->GetTransform();
	//spawn range..
	
	PowerShot = GetWorld()->SpawnActor<ACPowerShotObject>(PowerShotClass, transform, params);
	PowerShot->SetOwnerSkill(OwnerSkill);

	PowerShot->Action();
}
//
//void ACAbility_Fist_PowerShot::Special_Behavior()
//{
//	CheckNull(Inferno);
//
//	Inferno->OnCollision();
//}

void ACAbility_Fist_PowerShot::End_Behavior()
{
	//Inferno->Destroy();
	
	State->SetIdleMode();
	PowerShot->Destroy();
}

void ACAbility_Fist_PowerShot::SkillPreview()
{

}

void ACAbility_Fist_PowerShot::Cancel()
{
	bReady = false;
	bRangeCheck = false;
	State->SetIdleMode();
}
