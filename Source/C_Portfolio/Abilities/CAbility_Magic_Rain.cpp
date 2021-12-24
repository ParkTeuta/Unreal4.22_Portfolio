#include "CAbility_Magic_Rain.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Components/CSkillComponent.h"
#include "Components/CStateComponent.h"
#include "Weapons/CSkill.h"
#include "Engine/World.h"
#include "Engine/EngineTypes.h"

#include "GameFramework/CharacterMovementComponent.h"

#include "Weapons/CRainObject.h"

void ACAbility_Magic_Rain::BeginPlay()
{
	Super::BeginPlay();

	TArray<UShapeComponent*> components;
	OwnerSkill->GetShapeComponent(components);
	InputType = SkillInputType::Position;
	PreviewType = SkillPreviewType::Range;


}

void ACAbility_Magic_Rain::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

void ACAbility_Magic_Rain::Behavior()
{
	Super::Behavior();

	CheckFalse(OwnerSkill->GetbReady());
	State->SetSkillMode();

	OwnerCharacter->GetCharacterMovement()->StopMovementImmediately();
	FRotator rot = CHelpers::YawRotator(UKismetMathLibrary::FindLookAtRotation(OwnerCharacter->GetActorLocation(), OwnerSkill->GetSkillComp()->GetTargetPosition()));
	OwnerCharacter->SetActorRotation(CHelpers::YawRotator(UKismetMathLibrary::FindLookAtRotation(OwnerCharacter->GetActorLocation(), OwnerSkill->GetSkillComp()->GetTargetPosition())));
	
	OwnerCharacter->PlayAnimMontage(MontageData->AnimMontage, MontageData->PlayRatio, MontageData->StartSection);

}

void ACAbility_Magic_Rain::Begin_Behavior() //클릭하면
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
	ACRainObject *rain = GetWorld()->SpawnActor<ACRainObject>(Rain, transform, params);
	rain->SetOwnerSkill(OwnerSkill);
}

void ACAbility_Magic_Rain::End_Behavior()
{
	Cancel();
}

void ACAbility_Magic_Rain::SkillPreview()
{

}

void ACAbility_Magic_Rain::Cancel()
{
	bReady = false;
	bRangeCheck = false;
	State->SetIdleMode();
}
