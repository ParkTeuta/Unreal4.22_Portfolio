#include "CAbility_Magic_KnockBack.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Characters/CPlayer.h"
#include "Components/CSkillComponent.h"
#include "Components/CStateComponent.h"
#include "Components/ShapeComponent.h"
#include "Animation/AnimMontage.h"
#include "Weapons/CSkill.h"
#include "Weapons/CThrow_KnockBack.h"

void ACAbility_Magic_KnockBack::BeginPlay()
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

void ACAbility_Magic_KnockBack::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

void ACAbility_Magic_KnockBack::Behavior()
{
	Super::Behavior();

	CheckFalse(OwnerSkill->GetbReady());

	State->SetSkillMode();

	HitActors.Empty();
	ACPlayer* player = Cast<ACPlayer>(OwnerCharacter);
	player->StopMove();
	FRotator rot = CHelpers::YawRotator(UKismetMathLibrary::FindLookAtRotation(player->GetActorLocation(), OwnerSkill->GetSkillComp()->GetTargetPosition()));
	player->SetActorRotation(rot.Quaternion());

	FActorSpawnParameters params2;
	params2.Owner = OwnerCharacter;
	FTransform transform2;

	if (MagicCircleClass != NULL)
	{
		MagicCircle = Cast<AActor>(GetWorld()->SpawnActor(MagicCircleClass, &transform2, params2));
		MagicCircle->AttachToComponent(OwnerCharacter->GetMesh(),FAttachmentTransformRules::KeepRelativeTransform, L"Hand_R");
	}
	
	OwnerCharacter->PlayAnimMontage(MontageData->AnimMontage, MontageData->PlayRatio, MontageData->StartSection);
}
void ACAbility_Magic_KnockBack::Begin_Behavior() //클릭하면
{
	if (MagicCircle != NULL)
	{
		MagicCircle->Destroy();
		MagicCircle = NULL;
	}
	//spawn
	FActorSpawnParameters params;
	params.Owner = this;

	OwnerSkill->SetbReady(false);
	OwnerSkill->SetSkillCool();

	FTransform transform;
	transform.SetLocation(OwnerCharacter->GetActorLocation() + OwnerCharacter->GetActorForwardVector() * 30);
	transform.SetRotation(OwnerCharacter->GetActorRotation().Quaternion());

	OwnerCharacter->LaunchCharacter(OwnerCharacter->GetActorForwardVector() * -300.0f, false, false);

	//spawn throwitem..
	ACThrow_KnockBack *temp = GetWorld()->SpawnActor<ACThrow_KnockBack>(ThrowItem, transform, params);
	temp->SetOwnerSkill(OwnerSkill);
}

void ACAbility_Magic_KnockBack::End_Behavior()
{
	Cancel();

}

void ACAbility_Magic_KnockBack::SkillPreview()
{
}

void ACAbility_Magic_KnockBack::Cancel()
{
	if (MagicCircle != NULL)
	{
		MagicCircle->Destroy();
		MagicCircle = NULL;
	}

	bReady = false;
	bRangeCheck = false;

	State->SetIdleMode();
}
