#include "CAbility_Sword_WhirlWind.h"
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

void ACAbility_Sword_WhirlWind::BeginPlay()
{
	Super::BeginPlay();

	TArray<UShapeComponent*> components;
	OwnerSkill->GetShapeComponent(components);
	InputType = SkillInputType::Immediately;
	PreviewType = SkillPreviewType::Arrow;

	for (UShapeComponent* component : components)
	{
		component->OnComponentBeginOverlap.AddDynamic(OwnerSkill, &ACSkill::OnComponentBeginOverlap);
		component->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void ACAbility_Sword_WhirlWind::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

void ACAbility_Sword_WhirlWind::Behavior()
{
	Super::Behavior();
	CheckFalse(OwnerSkill->GetbReady());

	OwnerCharacter->GetCharacterMovement()->StopMovementImmediately();
	FRotator rot = CHelpers::YawRotator(UKismetMathLibrary::FindLookAtRotation(OwnerCharacter->GetActorLocation(), OwnerSkill->GetSkillComp()->GetTargetPosition()));
	OwnerCharacter->SetActorRotation(rot);

	State->SetSkillMode();

	OwnerCharacter->PlayAnimMontage(MontageData->AnimMontage, MontageData->PlayRatio, MontageData->StartSection);
}
void ACAbility_Sword_WhirlWind::Begin_Behavior()
{
	//spawn
	FActorSpawnParameters params;
	params.Owner = this;

	OwnerSkill->SetbReady(false);
	OwnerSkill->SetSkillCool();

}

void ACAbility_Sword_WhirlWind::End_Behavior()
{
	State->SetIdleMode();
	Cancel();
}


void ACAbility_Sword_WhirlWind::OnColliders()
{
	TArray<UShapeComponent*> components;
	OwnerSkill->GetShapeComponent(components);
	if (components.Num() != 0)
	{
		for (UShapeComponent* component : components)
		{
			component->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		}

	}
	else
	{
		for (UShapeComponent* component : OwnerSkill->GetClassColliders())
		{
			component->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		}
	}
}

void ACAbility_Sword_WhirlWind::OffColliders()
{
	TArray<UShapeComponent*> components;
	OwnerSkill->GetShapeComponent(components);
	if (components.Num() != 0)
	{
		for (UShapeComponent* component : components)
			component->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	else
	{
		for (UShapeComponent* component : OwnerSkill->GetClassColliders())
			component->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	HitActors.Empty();
}

void ACAbility_Sword_WhirlWind::PushForward()
{
	FVector vec = OwnerCharacter->GetActorForwardVector();
	vec.Z += 0.1f;

	CHelpers::SetCollisionPresetIgnoreOnlyPawn(OwnerCharacter->GetCapsuleComponent());
	//CHelpers::SetCollisionPresetIgnoreOnlyPawn(OwnerCharacter->GetMesh());
	
	OwnerCharacter->LaunchCharacter(vec * PushForwardPower, false, false);
}

void ACAbility_Sword_WhirlWind::SkillPreview()
{

}

void ACAbility_Sword_WhirlWind::Cancel()
{
	bReady = false;
	bRangeCheck = false;
	State->SetIdleMode();
	CHelpers::SetCollisionPresetPawn(OwnerCharacter->GetCapsuleComponent());
}
