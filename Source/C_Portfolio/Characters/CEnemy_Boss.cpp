#include "CEnemy_Boss.h"
#include "Global.h"
#include "Animation/AnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/TextRenderComponent.h"
//#include "Components/COptionComponent.h"
#include "Components/CStatusComponent.h"
#include "Components/CMontagesComponent.h"
#include "Weapons/CAttackObject.h"
#include "Weapons/CSkill.h"
#include "Materials/MaterialInstanceConstant.h"
#include "Materials/MaterialInstanceDynamic.h"

#include "CPlayer.h"
#include "TimerManager.h"

#include "Components/WidgetComponent.h"
#include "Blueprint/UserWidget.h"
#include "Widgets/CWidget_HpBar.h"

ACEnemy_Boss::ACEnemy_Boss()
{

	//Option = CreateDefaultSubobject<UCOptionComponent>("Option");
	State = CreateDefaultSubobject<UCStateComponent>("State");
	CC = CreateDefaultSubobject<UCCrowdControlComponent>("CC");
	Status = CreateDefaultSubobject<UCStatusComponent>("Status");
	Montages = CreateDefaultSubobject<UCMontagesComponent>("Montages");
	Skills = CreateDefaultSubobject<UCSkillComponent>("Skills");

	GetMesh()->RelativeLocation = FVector(0, 0, -90);
	GetMesh()->RelativeRotation = FRotator(0, -90, 0);

	USkeletalMesh* mesh;
	CHelpers::GetAsset<USkeletalMesh>(&mesh, "SkeletalMesh'/Game/Character/Enemy/BossEnemy/Skeletal_Meshes/Frank_2Hand_Skin.Frank_2Hand_Skin'");
	GetMesh()->SetSkeletalMesh(mesh);

	TSubclassOf<UAnimInstance> animInstance;
	CHelpers::GetClass<UAnimInstance>(&animInstance, "AnimBlueprint'/Game/Enemy/Boss/ABP_Boss.ABP_Boss_C'");
	GetMesh()->SetAnimInstanceClass(animInstance);

	UMaterialInstanceConstant* body;
	CHelpers::GetAsset<UMaterialInstanceConstant>(&body, "MaterialInstanceConstant'/Game/Character/Enemy/BossEnemy/MaterialInstance/Frank_2Hand_Body_Inst.Frank_2Hand_Body_Inst'");
	GetMesh()->SetMaterial(0, body);

	UMaterialInstanceConstant* M_Blade_GreatBlade;
	CHelpers::GetAsset<UMaterialInstanceConstant>(&M_Blade_GreatBlade, "MaterialInstanceConstant'/Game/Character/Enemy/BossEnemy/MaterialInstance/body_m_Inst.body_m_Inst'");
	GetMesh()->SetMaterial(1, M_Blade_GreatBlade);

	UMaterialInstanceConstant* Body_01;
	CHelpers::GetAsset<UMaterialInstanceConstant>(&Body_01, "MaterialInstanceConstant'/Game/Character/Enemy/BossEnemy/MaterialInstance/Frank_2Hand_Body_01_Inst.Frank_2Hand_Body_01_Inst'");
	GetMesh()->SetMaterial(2, Body_01);

	GetCharacterMovement()->RotationRate = FRotator(0, 720, 0);

	EnemyName = "Boss";

}

void ACEnemy_Boss::OnConstruction(const FTransform & Transform)
{
	Super::OnConstruction(Transform);

}


void ACEnemy_Boss::BeginPlay()
{

	Super::BeginPlay();
	GetCharacterMovement()->MaxWalkSpeed = Status->GetRunSpeed();

	BodyMaterial = UMaterialInstanceDynamic::Create(GetMesh()->GetMaterial(0), this);
	BladeMaterial = UMaterialInstanceDynamic::Create(GetMesh()->GetMaterial(1), this);
	Body2Material = UMaterialInstanceDynamic::Create(GetMesh()->GetMaterial(2), this);

	/*FMaterialParameterInfo paramInfo("BodyColor");

	BodyMaterial->GetVectorParameterValue
	(paramInfo, BodyColor);
	LogoMaterial->GetVectorParameterValue(paramInfo, LogoColor);*/

	GetMesh()->SetMaterial(0, BodyMaterial);
	GetMesh()->SetMaterial(1, BladeMaterial);
	GetMesh()->SetMaterial(2, Body2Material);

	State->OnStateTypeChanged.AddDynamic(this, &ACEnemy_Boss::OnStateTypeChanged);

	SpawnLocation = GetActorLocation();
	
	if (HpBarClass != NULL)
	{
		APlayerController* controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		HpBar = CreateWidget<UCWidget_HpBar>(controller, HpBarClass);
		if (HpBar)
		{
			HpBar->AddToViewport();
			HpBar->UpdateHealthBar(Status->GetHealth(), Status->GetMaxHealth());
			HpBar->UpdateName(EnemyName);
			HpBar->SetVisibility(ESlateVisibility::Collapsed);
		}
	}

}

void ACEnemy_Boss::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CC->IsCC())
	{
		CC->CCTick(DeltaTime);
	}
}


float ACEnemy_Boss::TakeDamage(float DamageAmount, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser)
{
	CheckTrueResult(State->IsDead(),0.0f);

	DamageInstigator = EventInstigator;
	DamageValue = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	ACAttackObject* attackObject = Cast<ACAttackObject>(DamageCauser);
	ACSkill* skill = (attackObject != NULL) ? attackObject->GetOwnerSkill() : Cast<ACSkill>(DamageCauser);
	if (skill != NULL)
	{
		DamageKnockBackPower = skill->GetKnockBackPower();
		DamageCC = skill->GetCC();
		DamageCCTime = skill->GetCCTime();
	}

	Hitted();
	//State->SetHittedMode();

	if (Status->GetHealth() <= 0.0f)
	{
		State->SetDeadMode();
		if (skill != NULL)
		{
			ACPlayer* player = Cast<ACPlayer>(skill->GetOwner());
			CheckNullResult(player, 0.0f);
			player->AddQuestItem(this);
		}

		return 0.0f;
	}

	return Status->GetHealth();
}

float ACEnemy_Boss::GetWalkSpeed()
{
	return Status->GetWalkSpeed();
}

float ACEnemy_Boss::GetRunSpeed()
{
	return Status->GetRunSpeed();
}

float ACEnemy_Boss::GetSprintSpeed()
{
	return Status->GetSprintSpeed();
}


void ACEnemy_Boss::OnStateTypeChanged(EStateType InPrevType, EStateType InNewType)
{
	switch (InNewType)
	{
		case EStateType::Dead: Dead(); return;
		case EStateType::Hitted: Hitted(); return;
	}

	//if (InNewType == EStateType::None)
	//{
	//	switch (InPrevType)
	//	{
	//	//case EStateType::Dead: End_Dead(); return;
	//	}
	//}
}

void ACEnemy_Boss::RestoreColor()
{/*
	BodyMaterial->SetVectorParameterValue("BodyColor", BodyColor);
	LogoMaterial->SetVectorParameterValue("BodyColor", LogoColor);*/
	BodyMaterial->SetVectorParameterValue("BodyColor", FLinearColor(1, 1, 1));
	BladeMaterial->SetVectorParameterValue("BodyColor", FLinearColor(1, 1, 1));
	Body2Material->SetVectorParameterValue("BodyColor", FLinearColor(1, 1, 1));
}

void ACEnemy_Boss::SetAggro(ACharacter * InCharacter)
{
	Aggro = InCharacter;
}

void ACEnemy_Boss::Attack()
{
	CheckNull(Aggro);
	Skills->AttackBehavior();
}

void ACEnemy_Boss::UseRandomSkill()
{
	TArray<ESkillButtonType> skillList = GetValidList();
	CheckFalse(skillList.Num() != 0);
	int32 num = FMath::RandRange(0, skillList.Num() - 1);
	Skills->SetSkill(skillList[num]);
	Skills->Behavior();
}

bool ACEnemy_Boss::CanSkillUse()
{
	if (GetValidList().Num() > 0)
		return true;
	return false;
}

void ACEnemy_Boss::OpenHpWidget()
{
	if (HpBar != NULL)
	{
		HpBar->SetVisibility(ESlateVisibility::Visible);
	}
}

void ACEnemy_Boss::CloseHpWidget()
{
	if (HpBar != NULL)
	{
		HpBar->SetVisibility(ESlateVisibility::Collapsed);
	}
}

TArray<ESkillButtonType> ACEnemy_Boss::GetValidList()
{
	TArray<ESkillButtonType> validList;
	for (FPhase phase : Phases)
	{
		float hpPercent = Status->GetHealth() / Status->GetMaxHealth() * 100.0f;
		if (phase.MinHpPercent <= hpPercent && phase.MaxHpPercent >= hpPercent)
		{
			ACSkill* skill = Skills->GetSkill(phase.SkillButton);
			if (skill->GetbReady())
				validList.Add(phase.SkillButton);
		}
	}

	return validList;
}

void ACEnemy_Boss::Hitted()
{
	Status->SubHealth(DamageValue);
	if (HpBar != NULL)
		HpBar->UpdateHealthBar(Status->GetHealth(), Status->GetMaxHealth());

	if (DamageCC != ECrowdControl::None)
	{
		CC->SetCCMode(DamageCC, DamageCCTime);

		const FMontageData_Character* data = Montages->GetHittedData();
		PlayAnimMontage(data->AnimMontage, data->PlayRatio, data->StartSection);

	}

	/*BodyMaterial->SetVectorParameterValue("BodyColor", FLinearColor(1, 0.5f, 0));
	LogoMaterial->SetVectorParameterValue("BodyColor", FLinearColor(1, 0.5f, 0));*/

	UKismetSystemLibrary::K2_SetTimer(this, "RestoreColor", 0.1f, false);

	DamageInstigator = NULL;
	DamageValue = 0.0f;

}

void ACEnemy_Boss::Dead()
{
	//GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
	if (HpBar != NULL)
		HpBar->SetVisibility(ESlateVisibility::Hidden);

	const FMontageData_Character* data = Montages->GetDeadData();
	PlayAnimMontage(data->AnimMontage, data->PlayRatio, data->StartSection);
}

void ACEnemy_Boss::End_Dead()
{
	//Destroy();
	
	GetWorldTimerManager().SetTimer(timer, this, &ACEnemy_Boss::Resurrection, 8.0f);
	GetMesh()->SetVisibility(false);
}


void ACEnemy_Boss::Resurrection()
{
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);

	StopAnimMontage();
	GetMesh()->SetVisibility(true);

	SetActorLocation(SpawnLocation);
	State->SetIdleMode();
	Status->ResetStatus();

	//Update HealthBar
	if (HpBar != NULL)
	{
		HpBar->SetVisibility(ESlateVisibility::Visible);
		HpBar->UpdateHealthBar(Status->GetHealth(), Status->GetMaxHealth());
	}

	GetWorldTimerManager().ClearTimer(timer);
}

void ACEnemy_Boss::SetBodyColor(FLinearColor InColor)
{
	BodyMaterial->SetVectorParameterValue("Param", InColor);
}

void ACEnemy_Boss::SetBladeColor(FLinearColor InColor)
{
	BladeMaterial ->SetVectorParameterValue("Param", InColor);
}

void ACEnemy_Boss::SetBody2Color(FLinearColor InColor)
{
	Body2Material->SetVectorParameterValue("Param", InColor);
}

FString ACEnemy_Boss::GetObjectName_Implementation()
{
	return EnemyName;
}

void ACEnemy_Boss::SetEnemyName(FString InName)
{
	EnemyName = InName;
}