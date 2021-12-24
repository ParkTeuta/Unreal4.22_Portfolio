#include "CAbility_Magic_Ultimate.h"
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

#include "Weapons/CRayAttack.h"

void ACAbility_Magic_Ultimate::BeginPlay()
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

void ACAbility_Magic_Ultimate::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

void ACAbility_Magic_Ultimate::Behavior()
{
	Super::Behavior();
	CheckFalse(OwnerSkill->GetbReady());

	OwnerCharacter->GetCharacterMovement()->StopMovementImmediately();
	FRotator rot = CHelpers::YawRotator(UKismetMathLibrary::FindLookAtRotation(OwnerCharacter->GetActorLocation(), OwnerSkill->GetSkillComp()->GetTargetPosition()));
	OwnerCharacter->SetActorRotation(CHelpers::YawRotator(UKismetMathLibrary::FindLookAtRotation(OwnerCharacter->GetActorLocation(), OwnerSkill->GetSkillComp()->GetTargetPosition())));

	State->SetSkillMode();

	if (HandAttackCircleClass != NULL)
	{
		FActorSpawnParameters params;
		params.Owner = OwnerCharacter;
		FTransform transform;

		HandAttachCircle = Cast<AActor>(GetWorld()->SpawnActor(HandAttackCircleClass, &transform, params));
		HandAttachCircle->AttachToComponent(OwnerCharacter->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, L"Hand_R");
	}

	OwnerCharacter->PlayAnimMontage(MontageData->AnimMontage, MontageData->PlayRatio, MontageData->StartSection);

}
void ACAbility_Magic_Ultimate::Begin_Behavior() //클릭하면
{
	//spawn
	OwnerSkill->SetbReady(false);
	OwnerSkill->SetSkillCool();

	if (GroundCircle1Class != NULL)
	{
		FActorSpawnParameters params;
		params.Owner = OwnerCharacter;
		FTransform transform;

		transform.SetLocation(OwnerCharacter->GetActorLocation() - FVector(0,0,30));
		GroundCircle1 = Cast<AActor>(GetWorld()->SpawnActor(GroundCircle1Class, &transform, params));
	}
}

void ACAbility_Magic_Ultimate::Middle_Behavior()
{
	if (GroundCircle2Class != NULL)
	{
		FActorSpawnParameters params;
		params.Owner = OwnerCharacter;
		FTransform transform;

		transform.SetLocation(OwnerCharacter->GetActorLocation() - FVector(0, 0, 35));
		GroundCircle2 = Cast<AActor>(GetWorld()->SpawnActor(GroundCircle2Class, &transform, params));
	}
}

void ACAbility_Magic_Ultimate::Special_Behavior()
{
	FActorSpawnParameters params;
	FTransform transform;
	transform.SetLocation(OwnerSkill->GetSkillComp()->GetTargetPosition());
	//spawn range..
	Ray = GetWorld()->SpawnActor<ACRayAttack>(RayClass, transform, params);
	Ray->SetOwnerSkill(OwnerSkill);
	Ray->SetLifeSpan(RayLifeTime);
}

void ACAbility_Magic_Ultimate::End_Behavior()
{
	State->SetIdleMode();
	Cancel();
}

void ACAbility_Magic_Ultimate::SkillPreview()
{

}

void ACAbility_Magic_Ultimate::Cancel()
{
	bReady = false;
	bRangeCheck = false;
	State->SetIdleMode();


	if (HandAttachCircle != NULL)
		HandAttachCircle->Destroy();
	if (GroundCircle1 != NULL)
		GroundCircle1->Destroy();
	if (GroundCircle2 != NULL)
		GroundCircle2->Destroy();

}
