#include "CAbility_Magic_Ray.h"
#include "Global.h"
#include "Weapons/CSkill.h"

//#include "Characters/CPlayer.h"
#include "Weapons/CRayAttack.h"

#include "GameFramework/CharacterMovementComponent.h"

#include "Components/CSkillComponent.h"
#include "Components/ShapeComponent.h"

void ACAbility_Magic_Ray::BeginPlay()
{
	Super::BeginPlay();

	/*TArray<UShapeComponent*> components;
	OwnerSkill->GetShapeComponent(components);*/
	InputType = SkillInputType::Holding;
	PreviewType = SkillPreviewType::Arrow;
}

void ACAbility_Magic_Ray::Tick(float DeltaTime) 
{

}

void ACAbility_Magic_Ray::Behavior()
{
	/*if (!bReady)
	{
		bReady = true;
		bRangeCheck = true;
		return;
	}*/

	Super::Behavior();

	CheckFalse(OwnerSkill->GetbReady());

	State->SetSkillMode();

	/*ACPlayer* player = Cast<ACPlayer>(OwnerCharacter);
	if (player != NULL)
		player->StopMove();*/

	OwnerCharacter->GetCharacterMovement()->StopMovementImmediately();

	OwnerCharacter->SetActorRotation(CHelpers::YawRotator(UKismetMathLibrary::FindLookAtRotation(OwnerCharacter->GetActorLocation(), OwnerSkill->GetSkillComp()->GetTargetPosition())));
	//OwnerSkill->GetSkillComp()->GetTargetPosition()


	OwnerCharacter->PlayAnimMontage(MontageData->AnimMontage, MontageData->PlayRatio, MontageData->StartSection);
	//HitActors.Empty();
}

void ACAbility_Magic_Ray::Begin_Behavior()
{
	//spawn
	FActorSpawnParameters params;
	params.Owner = this;

	bReady = true;

	FTransform transform;
	FVector handR = OwnerCharacter->GetMesh()->GetBoneLocation(L"Hand_R");
	FVector handL = OwnerCharacter->GetMesh()->GetBoneLocation(L"Hand_L");

	transform.SetLocation(FMath::Lerp(handR, handL, 0.5f));
	transform.SetRotation(OwnerCharacter->GetActorRotation().Quaternion());
	//transform.SetLocation(OwnerSkill->GetSkillComp()->GetTargetPosition());
	//spawn range..
	ray = GetWorld()->SpawnActor<ACRayAttack>(RayAttack, transform, params);
	ray->SetOwnerSkill(OwnerSkill);

	GetWorldTimerManager().SetTimer(timer, this, &ACAbility_Magic_Ray::End_Behavior, HoldingTime);
	/*ACRange *range = GetWorld()->SpawnActor<ACRange>(Range, transform, params);
	range->SetOwnerSkill(OwnerSkill);*/
}

void ACAbility_Magic_Ray::End_Behavior()
{
	if (bReady)
	{
		OwnerSkill->SetbReady(false);
		OwnerSkill->SetSkillCool();
	}

	if (ray != NULL)
		ray->Destroy();
	GetWorldTimerManager().ClearTimer(timer); 
	OwnerCharacter->PlayAnimMontage(MontageData->AnimMontage, MontageData->PlayRatio, "End");
	Cancel();
}

void ACAbility_Magic_Ray::Cancel()
{
	bReady = false;
	bRangeCheck = false;
	State->SetIdleMode();
	OwnerSkill->GetSkillComp()->SetIsHolding(false);
}
