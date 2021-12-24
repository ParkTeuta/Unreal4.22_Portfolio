#include "CEnemy.h"
#include "Global.h"
#include "Animation/AnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/TextRenderComponent.h"
#include "Components/WidgetComponent.h"
//#include "Components/COptionComponent.h"
#include "Components/CStatusComponent.h"
#include "Components/CMontagesComponent.h"
#include "Weapons/CAttackObject.h"
#include "Weapons/CSkill.h"
#include "Widgets/CWidget_HpBar.h"
#include "Materials/MaterialInstanceConstant.h"
#include "Materials/MaterialInstanceDynamic.h"

#include "CPlayer.h"
#include "TimerManager.h"

ACEnemy::ACEnemy()
{
	NameText = CreateDefaultSubobject<UTextRenderComponent>("NameText");
	NameText->SetupAttachment(GetMesh());

	HealthBar = CreateDefaultSubobject<UWidgetComponent>("HealthBar");
	HealthBar->SetupAttachment(GetMesh());

	//Option = CreateDefaultSubobject<UCOptionComponent>("Option");
	State = CreateDefaultSubobject<UCStateComponent>("State");
	CC = CreateDefaultSubobject<UCCrowdControlComponent>("CC");
	Status = CreateDefaultSubobject<UCStatusComponent>("Status");
	Montages = CreateDefaultSubobject<UCMontagesComponent>("Montages");
	Skills = CreateDefaultSubobject<UCSkillComponent>("Skills");

	GetMesh()->RelativeLocation = FVector(0, 0, -90);
	GetMesh()->RelativeRotation = FRotator(0, -90, 0);

	USkeletalMesh* mesh;
	CHelpers::GetAsset<USkeletalMesh>(&mesh, "SkeletalMesh'/Game/Character/Mesh/SK_Mannequin.SK_Mannequin'");
	GetMesh()->SetSkeletalMesh(mesh);

	TSubclassOf<UAnimInstance> animInstance;
	CHelpers::GetClass<UAnimInstance>(&animInstance, "AnimBlueprint'/Game/Enemy/ABP_Enemy.ABP_Enemy_C'");
	GetMesh()->SetAnimInstanceClass(animInstance);

	UMaterialInstanceConstant* body;
	CHelpers::GetAsset<UMaterialInstanceConstant>(&body, "MaterialInstanceConstant'/Game/Materials/M_Enemy_Body.M_Enemy_Body'");
	GetMesh()->SetMaterial(0, body);

	UMaterialInstanceConstant* logo;
	CHelpers::GetAsset<UMaterialInstanceConstant>(&logo, "MaterialInstanceConstant'/Game/Materials/M_Enemy_Logo.M_Enemy_Logo'");
	GetMesh()->SetMaterial(1, logo);

	GetCharacterMovement()->RotationRate = FRotator(0, 720, 0);

	NameText->RelativeLocation = FVector(0, 0, 190);
	NameText->RelativeRotation = FRotator(0, 90, 0);
	NameText->TextRenderColor = FColor::Red;
	NameText->HorizontalAlignment = EHorizTextAligment::EHTA_Center;

	TSubclassOf<UUserWidget> healthBarWidget;
	CHelpers::GetClass<UUserWidget>(&healthBarWidget, "WidgetBlueprint'/Game/Widgets/BP_CHealthBar.BP_CHealthBar_C'");
	HealthBar->SetWidgetClass(healthBarWidget);
	HealthBar->RelativeLocation = FVector(0, 0, 185);
	HealthBar->SetDrawSize(FVector2D(120, 15));
	HealthBar->SetWidgetSpace(EWidgetSpace::Screen);

	EnemyName = "Enemy";

}

void ACEnemy::OnConstruction(const FTransform & Transform)
{
	Super::OnConstruction(Transform);

	//FString name = GetName();
	//name = name.Replace(L"BP_CEnemy_", L"");
	//NameText->Text = FText::FromString(EnemyName);

}

float ACEnemy::TakeDamage(float DamageAmount, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser)
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
	State->SetHittedMode();

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

float ACEnemy::GetWalkSpeed()
{
	return Status->GetWalkSpeed();
}

float ACEnemy::GetRunSpeed()
{
	return Status->GetRunSpeed();
}

float ACEnemy::GetSprintSpeed()
{
	return Status->GetSprintSpeed();
}


void ACEnemy::BeginPlay()
{
	Super::BeginPlay();
	
	GetCharacterMovement()->MaxWalkSpeed = Status->GetRunSpeed();
	UCWidget_HpBar* healthBar = Cast<UCWidget_HpBar>(HealthBar->GetUserWidgetObject());
	healthBar->UpdateHealthBar(Status->GetHealth(), Status->GetMaxHealth());

	BodyMaterial = UMaterialInstanceDynamic::Create(GetMesh()->GetMaterial(0), this);
	LogoMaterial = UMaterialInstanceDynamic::Create(GetMesh()->GetMaterial(1), this);

	/*FMaterialParameterInfo paramInfo("BodyColor");

	BodyMaterial->GetVectorParameterValue
	(paramInfo, BodyColor);
	LogoMaterial->GetVectorParameterValue(paramInfo, LogoColor);*/

	GetMesh()->SetMaterial(0, BodyMaterial);
	GetMesh()->SetMaterial(1, LogoMaterial);

	State->OnStateTypeChanged.AddDynamic(this, &ACEnemy::OnStateTypeChanged);

	SpawnLocation = GetActorLocation();
}

void ACEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CC->IsCC())
	{
		CC->CCTick(DeltaTime);
	}
}

void ACEnemy::OnStateTypeChanged(EStateType InPrevType, EStateType InNewType)
{
	switch (InNewType)
	{
		case EStateType::Dead: Dead(); return;
		case EStateType::Hitted: Hitted(); return;
	}

	if (InNewType == EStateType::None)
	{
		switch (InPrevType)
		{
		case EStateType::Dead: End_Dead(); return;
		}
	}
}

void ACEnemy::RestoreColor()
{/*
	BodyMaterial->SetVectorParameterValue("BodyColor", BodyColor);
	LogoMaterial->SetVectorParameterValue("BodyColor", LogoColor);*/
	BodyMaterial->SetVectorParameterValue("BodyColor", FLinearColor(1, 1, 1));
	LogoMaterial->SetVectorParameterValue("BodyColor", FLinearColor(1, 1, 1));
}

void ACEnemy::SetAggro(ACharacter * InCharacter)
{
	Aggro = InCharacter;
}

void ACEnemy::Attack()
{
	CheckNull(Aggro);
	Skills->AttackBehavior();
}

void ACEnemy::UseSkill(ESkillButtonType InSkillButton)
{
	CheckNull(Skills);
	CheckNull(Aggro);
	Skills->SetSkill(InSkillButton);
	Skills->Behavior();
}

void ACEnemy::Hitted()
{

	Status->SubHealth(DamageValue);
	UCWidget_HpBar* healthBar = Cast<UCWidget_HpBar>(HealthBar->GetUserWidgetObject());
	healthBar->UpdateHealthBar(Status->GetHealth(), Status->GetMaxHealth());

	const FMontageData_Character* data = Montages->GetHittedData();
	PlayAnimMontage(data->AnimMontage, data->PlayRatio, data->StartSection);

	if (DamageCC != ECrowdControl::None)
	{
		CC->SetCCMode(DamageCC, DamageCCTime);
	}

	FVector start = this->GetActorLocation();
	FVector target = DamageInstigator->GetPawn()->GetActorLocation();
	FRotator rot = UKismetMathLibrary::FindLookAtRotation(start, target);
	rot = CHelpers::YawRotator(rot);
	SetActorRotation(rot);

	FVector velocity = -rot.Vector() * DamageKnockBackPower;
	LaunchCharacter(velocity, false, false);

	BodyMaterial->SetVectorParameterValue("BodyColor", FLinearColor(1, 0.5f, 0));
	LogoMaterial->SetVectorParameterValue("BodyColor", FLinearColor(1, 0.5f, 0));

	UKismetSystemLibrary::K2_SetTimer(this, "RestoreColor", 0.1f, false);

	DamageInstigator = NULL;
	DamageValue = 0.0f;

}

void ACEnemy::Dead()
{
	//GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);

	UCWidget_HpBar* healthBar = Cast<UCWidget_HpBar>(HealthBar->GetUserWidgetObject());
	healthBar->SetVisibility(ESlateVisibility::Hidden);

	const FMontageData_Character* data = Montages->GetDeadData();
	PlayAnimMontage(data->AnimMontage, data->PlayRatio, data->StartSection);
}

void ACEnemy::End_Dead()
{
	//Destroy();
	
	GetWorldTimerManager().SetTimer(timer, this, &ACEnemy::Resurrection, 8.0f);
	GetMesh()->SetVisibility(false);
}


void ACEnemy::Resurrection()
{
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);

	StopAnimMontage();
	GetMesh()->SetVisibility(true);

	SetActorLocation(SpawnLocation);
	State->SetIdleMode();
	Status->ResetStatus();

	//Update HealthBar
	UCWidget_HpBar* healthBar = Cast<UCWidget_HpBar>(HealthBar->GetUserWidgetObject());
	healthBar->SetVisibility(ESlateVisibility::Visible);
	healthBar->UpdateHealthBar(Status->GetHealth(), Status->GetMaxHealth());

	GetWorldTimerManager().ClearTimer(timer);
}

FString ACEnemy::GetObjectName_Implementation()
{
	return EnemyName;
}

void ACEnemy::SetEnemyName(FString InName)
{
	EnemyName = InName;
}