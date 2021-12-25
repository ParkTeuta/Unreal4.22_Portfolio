#include "CPlayer.h"
#include "Global.h"
#include "CPlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Animation/AnimInstance.h"
#include "Materials/Material.h"
#include "Camera/CameraComponent.h"
#include "Characters/CCamera.h"
#include "Components/DecalComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"

#include "Components/CStatusComponent.h"
#include "Components/CMontagesComponent.h"
#include "Components/CCrowdControlComponent.h"
#include "Components/CMoveInteractionComponent.h"

#include "Interactions/CMoveInteraction.h"

#include "Weapons/CSkill.h"
#include "Weapons/CAttackObject.h"
#include "PaperSpriteComponent.h"
#include "Materials/MaterialInstanceConstant.h"
#include "Materials/MaterialInstanceDynamic.h"

#include "CNpc.h"

#include "Widgets/CWidget_PlayerUI.h"
#include "Widgets/UCWidget_ElapsedBar.h"

ACPlayer::ACPlayer()
{
	PrimaryActorTick.bCanEverTick = true;

	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	//character mesh
	USkeletalMesh* mesh;
	CHelpers::GetAsset<USkeletalMesh>(&mesh, "SkeletalMesh'/Game/Character/Mesh/SK_Mannequin.SK_Mannequin'");
	GetMesh()->SetSkeletalMesh(mesh);

	GetMesh()->RelativeLocation = FVector(0, 0, -90);
	GetMesh()->RelativeRotation = FRotator(0, -90, 0);

	UMaterialInstanceConstant* body;
	CHelpers::GetAsset<UMaterialInstanceConstant>(&body, "MaterialInstanceConstant'/Game/Materials/M_Player_Body.M_Player_Body'");
	GetMesh()->SetMaterial(0, body);

	UMaterialInstanceConstant* logo;
	CHelpers::GetAsset<UMaterialInstanceConstant>(&logo, "MaterialInstanceConstant'/Game/Materials/M_Player_Logo.M_Player_Logo'");
	GetMesh()->SetMaterial(1, logo);


	//animation
	TSubclassOf<UAnimInstance> animInstance;
	CHelpers::GetClass<UAnimInstance>(&animInstance, "AnimBlueprint'/Game/Player/ABP_Player.ABP_Player_C'");
	GetMesh()->SetAnimInstanceClass(animInstance);

	//CharacterMovement
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	//SpringArm
	SpringArm = CreateDefaultSubobject<USpringArmComponent>("SpringArm");
	SpringArm->SetupAttachment(GetCapsuleComponent());
	SpringArm->TargetArmLength = 1200.f;
	SpringArm->RelativeRotation = FRotator(-70.f, 0.f, 0.f);
	SpringArm->bDoCollisionTest = false;
	SpringArm->bAbsoluteRotation = true;

	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	Camera->SetupAttachment(SpringArm);
	Camera->bUsePawnControlRotation = false;

	// Create a decal in the world to show the cursor's location
	CursorToWorld = CreateDefaultSubobject<UDecalComponent>("CursorToWorld");
	CursorToWorld->SetupAttachment(RootComponent);
	UMaterial* DecalMaterialAsset;
	CHelpers::GetAssetDynamic<UMaterial>(&DecalMaterialAsset, "Material'/Game/TopDownCPP/Blueprints/M_Cursor_Decal.M_Cursor_Decal'");
	if (!!DecalMaterialAsset)
	{
		CursorToWorld->SetDecalMaterial(DecalMaterialAsset);
	}
	CursorToWorld->DecalSize = FVector(16.0f, 32.0f, 32.0f);
	CursorToWorld->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f).Quaternion());


	// Activate ticking in order to update the cursor every frame.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;


	//Player Components
	State = CreateDefaultSubobject<UCStateComponent>("State");
	Status = CreateDefaultSubobject<UCStatusComponent>("Status");
	Skills = CreateDefaultSubobject<UCSkillComponent>("Skills");
	Montages = CreateDefaultSubobject<UCMontagesComponent>("Montages");
	QuestRecipient = CreateDefaultSubobject<UCRecipientComponent>("QuestRecipient");
	CClasses = CreateDefaultSubobject<UCClassComponent>("CClasses");
	CC = CreateDefaultSubobject<UCCrowdControlComponent>("CC");
	MoveInteractionComp = CreateDefaultSubobject<UCMoveInteractionComponent>("MoveInteractionComp");
}

void ACPlayer::BeginPlay()
{
	if (CCameraClass != NULL)
	{
		FActorSpawnParameters params;
		params.Owner = this;

		FreeCamera = GetWorld()->SpawnActor<ACCamera>(CCameraClass, params);

		SetFreeCameraToCamera();

		Camera->SetActive(false);

		UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetViewTarget(FreeCamera);
	}

	Super::BeginPlay();
	SetCameraMode(true);
	if (IsFirstStart)
	{
		Skills->SetSkillsCanUse(false);
		Status->ResetHealth();

		SetActorLocation(FVector(-22020, 0, 1962.0f));
	}

	UpdateHealthBar();
	RefreshQuestList();
}

UPaperSprite * ACPlayer::GetSprite(ESkillButtonType Input)
{
	CheckNullResult(Skills->GetSkill(Input),false);
	const TArray<FMontageData>* data = Skills->GetSkill(Input)->GetMontage()->GetAttackDatas();
	return data->GetData()[0].Sprite;
}

bool ACPlayer::GetSkillReady(ESkillButtonType Input)
{
	CheckNullResult(Skills->GetSkill(Input),false);
	return Skills->GetSkill(Input)->GetbReady();
}

int32 ACPlayer::GetSkillCoolTime(ESkillButtonType Input)
{
	CheckNullResult(Skills->GetSkill(Input), 0.0f);
	return Skills->GetSkill(Input)->GetSkillRemainCoolTime();
}

void ACPlayer::SetPlayerInfo(float InHealth, EClassType InClassType)
{
	if (InHealth == 0)
		return;
	Status->SetHealth(InHealth);

	switch (InClassType)
	{
	case EClassType::Unarmed:
		Skills->SetSkillsCanUse(false);
		break;
	case EClassType::Tutorial:
		OnTutorialClass();
		break;
	case EClassType::Fist:
		OnFist();
		break;
	case EClassType::Magic:
		OnMagic();
		break;
	case EClassType::Sword:
		OnSword();
		break;
	default:
		break;
	}

	IsFirstStart = false;
}

float ACPlayer::GetHealth()
{
	return Status->GetHealth();
}

void ACPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(Holding==true)
		SetFreeCameraToCamera();

	if (CursorToWorld != nullptr)
	{
		if (APlayerController* PC = Cast<APlayerController>(GetController()))
		{
			FHitResult TraceHitResult;
			PC->GetHitResultUnderCursor(ECC_Visibility, true, TraceHitResult);
			FVector CursorFV = TraceHitResult.ImpactNormal;
			FRotator CursorR = CursorFV.Rotation();
			CursorToWorld->SetWorldLocation(TraceHitResult.Location);
			CursorToWorld->SetWorldRotation(CursorR);

		}
	}

	TimeCount += DeltaTime;
	if (TickTime <= TimeCount)
	{
		Status->AddHealth(AutoHeal);
		TimeCount = 0.0f;
	}

	if (State->IsAttack())
	{

	}

}

void ACPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

float ACPlayer::TakeDamage(float DamageAmount, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser)
{
	CheckTrueResult(State->IsDead(), 0.0f);

	DamageInstigator = EventInstigator;
	DamageValue = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	Status->SubHealth(DamageValue);
	UpdateHealthBar();

	QuestRecipient->CancelInteraction();

	if (Status->GetHealth() <= 0.0f)
	{
		State->SetDeadMode();

		return 0.0f;
	}

	return Status->GetHealth();
}

void ACPlayer::OnStateTypeChanged(EStateType InPrevType, EStateType InNewType)
{
	switch (InNewType)
	{
	case EStateType::Dead: Dead(); return;
	}
	if (InNewType == EStateType::None)
	{
		switch (InPrevType)
		{
		case EStateType::Dead: GameBadEnding(); return;
		}
	}
}

bool ACPlayer::GetSkillsCanUse()
{
	return Skills->GetSkillsCanUse();
}

void ACPlayer::SetSkillButton(ESkillButtonType InNewButton)
{
	Skills->SetSkill(InNewButton);
}

void ACPlayer::SetSkillReleased(ESkillButtonType InNewButton)
{
	Skills->SkillReleased(InNewButton);
}

bool ACPlayer::GetSKillIsHolding(ESkillButtonType InNewButton)
{
	return Skills->GetSkillIsHolding(InNewButton);
}

bool ACPlayer::GetSKillIsImmediately(ESkillButtonType InNewButton)
{
	return Skills->GetSkillIsImmediately(InNewButton);
}

void ACPlayer::StopMove()
{
	GetCharacterMovement()->StopMovementImmediately();
}


void ACPlayer::SetCameraMode(bool InCameraMode)
{
	if (InCameraMode)
	{//camera
		Camera->SetActive(true);
		Holding = true;
		//FreeCamera->GetCameraComponent()->SetActive(false);
	}
	else
	{//freeCamera
		Camera->SetActive(false);
		Holding = false;
		//FreeCamera->GetCameraComponent()->SetActive(true);
	}
	FreeCamera->SetHolding(Holding);
}

void ACPlayer::SetCameraZoom(float Axis)
{
	if (Axis == 0)
		return;
	if (Axis < 0 && SpringArm->TargetArmLength >= 1200)
		return;
	if (Axis > 0 && SpringArm->TargetArmLength <= 700)
		return;

	SpringArm->TargetArmLength = SpringArm->TargetArmLength - (Axis * 20.0f);

	SetFreeCameraToCameraZ();
}

void ACPlayer::Dead()
{
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);

	PlayerUIWidget->SetVisibility(ESlateVisibility::Collapsed);

	const FMontageData_Character* data = Montages->GetDeadData();
	PlayAnimMontage(data->AnimMontage, data->PlayRatio, data->StartSection);

	ExtraDead();
}

void ACPlayer::SetAttackTarget(ACharacter* InEnemy)
{
	Enemy = InEnemy;
}


void ACPlayer::Attack()
{
	StopMove();
	SetActorRotation(CHelpers::YawRotator(UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), CLocation)).Quaternion());
	Skills->AttackBehavior();
}

void ACPlayer::Action()
{
	Skills->Behavior();
}

void ACPlayer::SkillCancel()
{
	Skills->Cancel();
}

void ACPlayer::OnTutorialClass()
{
	CheckFalse(State->IsIdle());

	CClasses->SetMode(EClassType::Tutorial);
	UpdateSkillSprite();
	UpdateClassSprite(CClasses->GetCClassSprite());
}

void ACPlayer::OnMagic()
{
	CheckFalse(State->IsIdle());

	CClasses->SetMode(EClassType::Magic);
	UpdateSkillSprite();
	UpdateClassSprite(CClasses->GetCClassSprite());
}

void ACPlayer::OnFist()
{
	CheckFalse(State->IsIdle());

	CClasses->SetMode(EClassType::Fist);
	UpdateSkillSprite();
	UpdateClassSprite(CClasses->GetCClassSprite());
}

void ACPlayer::OnSword()
{
	CheckFalse(State->IsIdle());

	CClasses->SetMode(EClassType::Sword);
	UpdateSkillSprite();
	UpdateClassSprite(CClasses->GetCClassSprite());
}

void ACPlayer::ChangeCClass()
{
	if (CClasses->IsFist())
		OnSword();
	else if (CClasses->IsSword())
		OnFist();
}

void ACPlayer::MoveToTarget()
{
	UAIBlueprintHelperLibrary::SimpleMoveToLocation(this->GetController(), Enemy->GetActorLocation());
}

void ACPlayer::MoveToTarget(FVector InTargetPosition)
{
	UAIBlueprintHelperLibrary::SimpleMoveToLocation(this->GetController(), InTargetPosition);
}

void ACPlayer::RefreshQuestList_Implementation()
{

}

void ACPlayer::RefreshQuestItem_Implementation(int32 InCount)
{

}

void ACPlayer::AddQuestItem(AActor * InActor, int32 InCount)
{
	QuestRecipient->AddCount(InActor,InCount);

	//뭔가 얻게 되면 다 여기서 처리할거
	
}

void ACPlayer::SetPlayerData(ACPlayer * InPlayer)
{
	CheckNull(InPlayer);

	
}

void ACPlayer::SetFreeCameraToCamera()
{
	CheckFalse(FreeCamera!=NULL);
	FreeCamera->SetCameraLocationAndRotation(Camera->GetComponentLocation(), Camera->GetComponentRotation());
}

void ACPlayer::SetFreeCameraToCameraZ()
{
	CheckFalse(FreeCamera != NULL);
	FVector location = FreeCamera->GetActorLocation();
	location.Z = Camera->GetComponentLocation().Z;
	FreeCamera->SetCameraLocationAndRotation(location, Camera->GetComponentRotation());
}

void ACPlayer::SetPlayerUIWidget(UCWidget_PlayerUI *InWidget)
{
	PlayerUIWidget = InWidget;
}

void ACPlayer::UpdateSkillSprite()
{
	if (PlayerUIWidget != NULL)
		PlayerUIWidget->UpdateSkillSprite();
}

void ACPlayer::UpdateSkillSpriteInSkillButton(ESkillButtonType InSkillButton, UPaperSprite * InSprite)
{
	if (PlayerUIWidget != NULL && InSprite != NULL)
		PlayerUIWidget->UpdateSkillSpriteInSkillButton(InSkillButton, InSprite);
}

void ACPlayer::UpdateClassSprite(UPaperSprite * InSprite)
{
	if (PlayerUIWidget != NULL && InSprite != NULL)
		PlayerUIWidget->UpdateClassSprite(InSprite);
}

void ACPlayer::UpdateHealthBar()
{
	if (PlayerUIWidget != NULL)
		PlayerUIWidget->UpdateHealthBar(Status->GetHealth(), Status->GetMaxHealth());
}

void ACPlayer::SetPlayerUIVisibility(ESlateVisibility InVisibility)
{
	PlayerUIWidget->SetVisibility(InVisibility);
}

bool ACPlayer::CanActionMoveInteraction()
{
	return MoveInteractionComp->CanActionMoveInteraction();
}

void ACPlayer::StartMoveInteraction()
{
	MoveInteractionComp->Start();

}

void ACPlayer::EndMoveInteraction()
{
	MoveInteractionComp->End();
}
