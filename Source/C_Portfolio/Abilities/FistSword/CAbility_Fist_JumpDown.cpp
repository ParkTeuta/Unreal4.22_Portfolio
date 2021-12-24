#include "CAbility_Fist_JumpDown.h"
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

#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Weapons/FistSword/CJumpDownObject.h"

void ACAbility_Fist_JumpDown::BeginPlay()
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

void ACAbility_Fist_JumpDown::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

void ACAbility_Fist_JumpDown::Behavior()
{
	Super::Behavior();
	CheckFalse(OwnerSkill->GetbReady());

	OwnerCharacter->GetCharacterMovement()->StopMovementImmediately();
	FRotator rot = CHelpers::YawRotator(UKismetMathLibrary::FindLookAtRotation(OwnerCharacter->GetActorLocation(), OwnerSkill->GetSkillComp()->GetTargetPosition()));
	OwnerCharacter->SetActorRotation(rot);

	State->SetSkillMode();

	OwnerCharacter->LaunchCharacter(FVector(0, 0, 1) * JumpPower, false, false);
	OwnerCharacter->PlayAnimMontage(MontageData->AnimMontage, MontageData->PlayRatio, MontageData->StartSection);
}
void ACAbility_Fist_JumpDown::Begin_Behavior()
{
	//spawn
	FActorSpawnParameters params;
	params.Owner = this;

	OwnerSkill->SetbReady(false);
	OwnerSkill->SetSkillCool();

	OwnerCharacter->GetCharacterMovement()->StopMovementImmediately();
	FRotator rot = UKismetMathLibrary::FindLookAtRotation(OwnerCharacter->GetActorLocation(), OwnerSkill->GetSkillComp()->GetTargetPosition());
	OwnerCharacter->LaunchCharacter(rot.Vector() * DownPower, false, false);

	CHelpers::SetCollisionPresetOverlapOnlyPawn(OwnerCharacter->GetCapsuleComponent());

	FTransform transform;
	transform = OwnerCharacter->GetTransform();

	//JumpDown = GetWorld()->Spawn
}


void ACAbility_Fist_JumpDown::End_Behavior()
{
	CHelpers::SetCollisionPresetPawn(OwnerCharacter->GetCapsuleComponent());
	State->SetIdleMode();
	Cancel();
}

void ACAbility_Fist_JumpDown::OnColliders()
{
	TArray<UShapeComponent*> components;
	OwnerSkill->GetShapeComponent(components);
	for (UShapeComponent* component : components)
		component->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

void ACAbility_Fist_JumpDown::OffColliders()
{
	TArray<UShapeComponent*> components;
	OwnerSkill->GetShapeComponent(components);
	for (UShapeComponent* component : components)
		component->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ACAbility_Fist_JumpDown::SkillPreview()
{

}

void ACAbility_Fist_JumpDown::Cancel()
{
	CHelpers::SetCollisionPresetPawn(OwnerCharacter->GetCapsuleComponent());
	bReady = false;
	bRangeCheck = false;
	State->SetIdleMode();
}
