#include "CAbility_Fist_EnergyShot.h"
#include "Global.h"
#include "Weapons/CSkill.h"

#include "Weapons/CAttackObject.h"

#include "GameFramework/CharacterMovementComponent.h"

#include "Components/CSkillComponent.h"
#include "Components/ShapeComponent.h"

void ACAbility_Fist_EnergyShot::BeginPlay()
{
	Super::BeginPlay();

	InputType = SkillInputType::Holding;
	PreviewType = SkillPreviewType::Arrow;
}

void ACAbility_Fist_EnergyShot::Tick(float DeltaTime) 
{
	if (IsCharging)
	{
		ChargingTime += DeltaTime;
		if (ChargingTime >= HoldingTime)
		{
			ChargingTime = HoldingTime;
			IsCharging = false;
		}
	}
}

void ACAbility_Fist_EnergyShot::Behavior()
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
	ChargingTime = 0.0f;

	OwnerCharacter->SetActorRotation(CHelpers::YawRotator(UKismetMathLibrary::FindLookAtRotation(OwnerCharacter->GetActorLocation(), OwnerSkill->GetSkillComp()->GetTargetPosition())));
	//OwnerSkill->GetSkillComp()->GetTargetPosition()


	OwnerCharacter->PlayAnimMontage(MontageData->AnimMontage, MontageData->PlayRatio, MontageData->StartSection);
	//HitActors.Empty();
}

void ACAbility_Fist_EnergyShot::Begin_Behavior()
{
	//spawn
	bReady = true;

	//transform.SetLocation(OwnerSkill->GetSkillComp()->GetTargetPosition());
	//spawn range..

	IsCharging = true;
	GetWorldTimerManager().SetTimer(timer, this, &ACAbility_Fist_EnergyShot::FinishCharge, HoldingTime);
	ZoomIn();
}

void ACAbility_Fist_EnergyShot::Middle_Behavior()
{
	CheckFalse(bReady);
	FActorSpawnParameters params;
	params.Owner = this;


	FTransform transform;
	FVector handR = OwnerCharacter->GetMesh()->GetBoneLocation(L"Hand_R");
	FVector handL = OwnerCharacter->GetMesh()->GetBoneLocation(L"Hand_L");

	transform.SetLocation(FMath::Lerp(handR, handL, 0.5f));
	transform.SetRotation(OwnerCharacter->GetActorRotation().Quaternion());

	Ray = GetWorld()->SpawnActor<ACAttackObject>(RayAttack, transform, params);
	CalculationPower();

	Ray->SetOwnerSkill(OwnerSkill);

	Cancel();
	
}

void ACAbility_Fist_EnergyShot::End_Behavior()
{
	GetWorldTimerManager().ClearTimer(timer);
	if (bReady)
	{
		OwnerSkill->SetbReady(false);
		OwnerSkill->SetSkillCool();
	}

	OwnerCharacter->PlayAnimMontage(MontageData->AnimMontage, MontageData->PlayRatio, "End");
	ZoomInStop();
	ZoomOut();
}

void ACAbility_Fist_EnergyShot::Cancel()
{
	bReady = false;
	bRangeCheck = false;
	State->SetIdleMode();
	OwnerSkill->GetSkillComp()->SetIsHolding(false);
	ChargingTime = 0.0f;
}

float ACAbility_Fist_EnergyShot::GetPower()
{
	return thisPower;
}

void ACAbility_Fist_EnergyShot::FinishCharge()
{
	IsCharging = false;
	ChargingTime = HoldingTime;
	End_Behavior();
}

float ACAbility_Fist_EnergyShot::CalculationPower()
{
	float TimeRatio = ChargingTime / HoldingTime;
	float DamageRatio = PowerMax - PowerMin;
	thisPower = PowerMin + (DamageRatio * TimeRatio);
	return thisPower;
}
