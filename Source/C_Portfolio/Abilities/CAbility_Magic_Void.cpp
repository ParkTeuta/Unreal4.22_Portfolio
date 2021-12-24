#include "CAbility_Magic_Void.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Characters/CPlayer.h"
#include "Components/CSkillComponent.h"
#include "Components/CStateComponent.h"
#include "Components/ShapeComponent.h"
#include "Animation/AnimMontage.h"
#include "Weapons/CSkill.h"
#include "Weapons/CThrow_Void.h"

void ACAbility_Magic_Void::BeginPlay()
{
	Super::BeginPlay();

	InputType = SkillInputType::Position;
	PreviewType = SkillPreviewType::Arrow;
}

void ACAbility_Magic_Void::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

void ACAbility_Magic_Void::Behavior()
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
void ACAbility_Magic_Void::Begin_Behavior() //클릭하면
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
	transform.SetLocation(OwnerCharacter->GetMesh()->GetBoneLocation(L"Hand_R"));
	transform.SetRotation(OwnerCharacter->GetActorRotation().Quaternion());
	//spawn throwitem..
	ACThrow_Void *temp = GetWorld()->SpawnActor<ACThrow_Void>(ThrowItem, transform, params);
	temp->SetOwnerSkill(OwnerSkill);
}

void ACAbility_Magic_Void::End_Behavior()
{
	Cancel();

}

void ACAbility_Magic_Void::SkillPreview()
{
}

void ACAbility_Magic_Void::Cancel()
{
	

	bReady = false;
	bRangeCheck = false;

	State->SetIdleMode();
}
