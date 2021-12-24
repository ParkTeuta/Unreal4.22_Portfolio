#include "CAbility_Magic_Inferno.h"
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

#include "Weapons/CInfernoObject.h"

void ACAbility_Magic_Inferno::BeginPlay()
{
	Super::BeginPlay();

	TArray<UShapeComponent*> components;
	OwnerSkill->GetShapeComponent(components);
	InputType = SkillInputType::Position;
	PreviewType = SkillPreviewType::Range;


}

void ACAbility_Magic_Inferno::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

void ACAbility_Magic_Inferno::Behavior()
{
	Super::Behavior();
	CheckFalse(OwnerSkill->GetbReady());

	OwnerCharacter->GetCharacterMovement()->StopMovementImmediately();
	FRotator rot = CHelpers::YawRotator(UKismetMathLibrary::FindLookAtRotation(OwnerCharacter->GetActorLocation(), OwnerSkill->GetSkillComp()->GetTargetPosition()));
	OwnerCharacter->SetActorRotation(CHelpers::YawRotator(UKismetMathLibrary::FindLookAtRotation(OwnerCharacter->GetActorLocation(), OwnerSkill->GetSkillComp()->GetTargetPosition())));

	State->SetSkillMode();

	OwnerCharacter->PlayAnimMontage(MontageData->AnimMontage, MontageData->PlayRatio, MontageData->StartSection);
}
void ACAbility_Magic_Inferno::Begin_Behavior() //클릭하면
{
	//spawn
	FActorSpawnParameters params;
	params.Owner = this;

	OwnerSkill->SetbReady(false);
	OwnerSkill->SetSkillCool();

	FTransform transform;
	transform.SetLocation(OwnerSkill->GetSkillComp()->GetTargetPosition());
	//spawn range..
	Inferno = GetWorld()->SpawnActor<ACInfernoObject>(InfernoClass, transform, params);
	Inferno->SetOwnerSkill(OwnerSkill);

}

void ACAbility_Magic_Inferno::Special_Behavior()
{
	CheckNull(Inferno);

	Inferno->OnCollision();
}

void ACAbility_Magic_Inferno::End_Behavior()
{
	//Inferno->Destroy();
	
	State->SetIdleMode();
	Inferno->SetLifeSpan(0.8f);
}

void ACAbility_Magic_Inferno::SkillPreview()
{

}

void ACAbility_Magic_Inferno::Cancel()
{
	bReady = false;
	bRangeCheck = false;
	State->SetIdleMode();
}
