#include "CPlayerController.h"
#include "Global.h"
#include "CPlayer.h"
#include "CNpc.h"
#include "Components/InputComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
//#include "Components/COptionComponent.h"
#include "Components/CRecipientComponent.h"
#include "Components/CStateComponent.h"
#include "Components/CStatusComponent.h"
#include "Components/CSkillComponent.h"
#include "Weapons/CSkill.h"

#include "Objects/CInteractionObject.h"

#include "Blueprint/UserWidget.h"
#include "Widgets/CWidget_PlayerUI.h"
#include "Widgets/CWidget_Dialogue.h"
#include "Widgets/UCWidget_ElapsedBar.h"


ACPlayerController::ACPlayerController()
{
	//Cursor모습 보이기
	bShowMouseCursor = true;
	//DefaultMouseCursor = EMouseCursor::Crosshairs;

	PrimaryActorTick.bCanEverTick = true;

	Input = CreateDefaultSubobject<UInputComponent>("Input");
}

void ACPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
	
	if (bMoveToMouseCursor)
	{
		MoveToMouseCursor();
	}
}

void ACPlayerController::BeginPlay()
{
	Super::BeginPlay();
}

void ACPlayerController::OnPossess(APawn * aPawn)
{
	Super::OnPossess(aPawn);

	InputComponent = Input;

	Player = Cast<ACPlayer>(aPawn);
	State = Player->GetState();
	Status = Player->GetStatus();


	if (PlayerUIClass != NULL) //SkillWidget Setting, AddVieport
	{
		PlayerUI = CreateWidget<UCWidget_PlayerUI>(this, PlayerUIClass);
		if (PlayerUI != NULL)
		{
			PlayerUI->AddToViewport();
			PlayerUI->SetPlayer(Player);
			Player->SetPlayerUIWidget(PlayerUI);
			
		}
	}
	
	if (DialogueClass != NULL)
	{
		DialogueUI = CreateWidget<UCWidget_Dialogue>(this, DialogueClass);
		if (DialogueUI != NULL)
		{
			DialogueUI->AddToViewport();
			DialogueUI->SetVisibility(ESlateVisibility::Collapsed);

		}
	}

	if (InteractionBarClass != NULL)
	{
		InteractionBar = CreateWidget<UUCWidget_ElapsedBar>(this, InteractionBarClass);
		if (InteractionBar != NULL)
		{
			InteractionBar->AddToViewport();
			InteractionBar->SetVisibility(ESlateVisibility::Collapsed);
			Player->SetInteractionBar(InteractionBar);
		}
	}

}

void ACPlayerController::OnUnPossess()
{
	Super::OnUnPossess();

}

void ACPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	//mouse wheel
	Input->BindAxis("Zoom", this, &ACPlayerController::ZoomInOut);

	//Mouse Right Click
	Input->BindAction("SetDestination", IE_Pressed, this, &ACPlayerController::OnSetDestinationPressed);
	Input->BindAction("SetDestination", IE_Released, this, &ACPlayerController::OnSetDestinationReleased);

	//Mouse Left Click
	Input->BindAction("UseAction", IE_Pressed, this, &ACPlayerController::OnUseActionPressed);

	//Keyboard Q
	Input->BindAction("Q", IE_Pressed, this, &ACPlayerController::OnQSkillPressed);
	Input->BindAction("Q", IE_Released, this, &ACPlayerController::OnQSkillReleased);

	//Keyboard W
	Input->BindAction("W", IE_Pressed, this, &ACPlayerController::OnWSkillPressed);
	Input->BindAction("W", IE_Released, this, &ACPlayerController::OnWSkillReleased);

	//Keyboard E
	Input->BindAction("E", IE_Pressed, this, &ACPlayerController::OnESkillPressed);
	Input->BindAction("E", IE_Released, this, &ACPlayerController::OnESkillReleased);

	//Keyboard R
	Input->BindAction("R", IE_Pressed, this, &ACPlayerController::OnRSkillPressed);
	Input->BindAction("R", IE_Released, this, &ACPlayerController::OnRSkillReleased);

	//Keyboard A
	Input->BindAction("A", IE_Pressed, this, &ACPlayerController::OnASkillPressed);
	Input->BindAction("A", IE_Released, this, &ACPlayerController::OnASkillReleased);

	//Keyboard S
	Input->BindAction("S", IE_Pressed, this, &ACPlayerController::OnSSkillPressed);
	Input->BindAction("S", IE_Released, this, &ACPlayerController::OnSSkillReleased);

	//Keyboard D
	Input->BindAction("D", IE_Pressed, this, &ACPlayerController::OnDSkillPressed);
	Input->BindAction("D", IE_Released, this, &ACPlayerController::OnDSkillReleased);

	//Keyboard F
	Input->BindAction("F", IE_Pressed, this, &ACPlayerController::OnFSkillPressed);
	Input->BindAction("F", IE_Released, this, &ACPlayerController::OnFSkillReleased);

	//Keyboard Z
	Input->BindAction("Z", IE_Pressed, this, &ACPlayerController::OnZSkillPressed);

	//Keyboard SpaceBar
	Input->BindAction("Dash", IE_Pressed, this, &ACPlayerController::OnDashSkillPressed);
	Input->BindAction("Dash", IE_Released, this, &ACPlayerController::OnDashSkillReleased);

	Input->BindAction("Interaction", IE_Pressed, this, &ACPlayerController::InteractionPressed);
}

void ACPlayerController::MoveToMouseCursor()
{ //우클릭
	CheckTrue(Player->GetRecipient()->GetIsWidgetOpen()); //Quest위젯 열려있는지
	CheckFalse(State->IsIdle()); //Player가 Idle 상태인지.

	//Player->SkillCancel();

	// Trace to see what is under the mouse cursor
	FHitResult Hit;

	//땅이 클릭됐는지
	//ObjectTypeQuery1 - WorldStatic
	//ObjectTypeQuery2 - WorldDynamic
	//ObjectTypeQuery3 - Pawn
	//ObjectTypeQuery4 - PhysicsBody
	TArray<TEnumAsByte<EObjectTypeQuery>> objectType;
	objectType.Add(ObjectTypeQuery1); //WorldStatic
	GetHitResultUnderCursorForObjects(objectType, false, Hit);

	if (Hit.bBlockingHit)
	{
		Player->GetState()->SetIdleMode();
		Player->GetRecipient()->CancelInteraction();
		Player->AttackCancel();
		Player->SetAttackTarget(NULL);
		Player->SetBaseAttack(false);
		// We hit something, move there
		SetNewMoveDestination(Hit.ImpactPoint);
	}
}

void ACPlayerController::SetNewMoveDestination(const FVector DestLocation)
{
	APawn* const MyPawn = GetPawn();
	if (MyPawn)
	{
		float const Distance = FVector::Dist(DestLocation, MyPawn->GetActorLocation());

		// We need to issue move command only if far enough in order for walk animation to play correctly
		if ((Distance > 120.0f))
		{
			UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, DestLocation);
		}
	}
}

void ACPlayerController::OnSetDestinationPressed()
{
	bMoveToMouseCursor = true;
}

void ACPlayerController::OnSetDestinationReleased()
{
	bMoveToMouseCursor = false;
}

void ACPlayerController::OnUseActionPressed() 
{//왼클릭
	CheckTrue(Player->GetRecipient()->GetIsWidgetOpen());
	CheckTrue(State->IsSkill());

	//스킬 쓰려는 게 준비돼있으면 스킬 시전, 아니면 평타
	ACSkill* waitSkill = Player->GetSkillWait(); 
	if (waitSkill != NULL)
	{
		FHitResult Hit;
		if (waitSkill->GetInputType() == SkillInputType::Enemy)
		{
			//Pawn이 클릭됐는지
			TArray<TEnumAsByte<EObjectTypeQuery>> objectType;
			objectType.Add(ObjectTypeQuery3); //Pawn
			GetHitResultUnderCursorForObjects(objectType, false, Hit);

			if (Hit.bBlockingHit)
			{
				if (!!Cast<ACharacter>(Hit.GetActor()) && Cast<ACPlayer>(Hit.GetActor()) != Player) 
				{ //클릭한게 ACharacter인지 확인, Player를 클릭한 건 아닌지 확인
					Player->SetSkillEnemy(Cast<ACharacter>(Hit.GetActor())); 
					Player->SetTargetPosition(Cast<ACharacter>(Hit.GetActor())->GetActorLocation());
					Player->Action();
					DefaultMouseCursor = EMouseCursor::Default;
					return;
				}
				else
				{
					Player->SetSkillEnemy(NULL);
				}
				//Player->Action();
				return;
			}
		}
		else if (waitSkill->GetInputType() == SkillInputType::Position)
		{
			TArray<TEnumAsByte<EObjectTypeQuery>> objectType;
			objectType.Add(ObjectTypeQuery1);
			GetHitResultUnderCursorForObjects(objectType, false, Hit);
			if (Hit.bBlockingHit)
			{
				Player->SetTargetPosition(Hit.ImpactPoint);
				Player->Action();
				return;
			}
		}

	Player->SkillCancel();
	}
	else
	{
		FHitResult Hit;

		//땅이 클릭됐는지
		TArray<TEnumAsByte<EObjectTypeQuery>> objectType;
		objectType.Add(ObjectTypeQuery1);
		GetHitResultUnderCursorForObjects(objectType, false, Hit);

		if (Hit.bBlockingHit)
		{
				Player->SetCLocation(Hit.ImpactPoint);
				Player->SetBaseAttack(true);
				Player->Attack();
				return;
		}
	}
}

void ACPlayerController::OnQSkillPressed()
{
	SkillPressed(ESkillButtonType::Q);
}

void ACPlayerController::OnWSkillPressed()
{
	SkillPressed(ESkillButtonType::W);
}

void ACPlayerController::OnESkillPressed()
{
	SkillPressed(ESkillButtonType::E);
}

void ACPlayerController::OnRSkillPressed()
{
	SkillPressed(ESkillButtonType::R);
}

void ACPlayerController::OnASkillPressed()
{
	SkillPressed(ESkillButtonType::A);
}

void ACPlayerController::OnSSkillPressed()
{
	SkillPressed(ESkillButtonType::S);
}

void ACPlayerController::OnDSkillPressed()
{
	SkillPressed(ESkillButtonType::D);
}

void ACPlayerController::OnFSkillPressed()
{
	SkillPressed(ESkillButtonType::F);
}

void ACPlayerController::OnZSkillPressed()
{
	//SkillPressed(ESkillButtonType::R);
	Player->ChangeCClass();
}

void ACPlayerController::OnDashSkillPressed()
{
	CheckTrue(Player->GetRecipient()->GetIsWidgetOpen());
	CheckFalse(State->IsIdle());

	if (State->IsSkill())
		Player->SkillCancel();
	if (State->IsAttack())
		Player->AttackCancel();

	Player->SetSkillButton(ESkillButtonType::Dash);
	FHitResult Hit;
	GetHitResultUnderCursor(ECC_Visibility, false, Hit);
	if (Hit.bBlockingHit)
	{
		Player->SetTargetPosition(Hit.ImpactPoint);
		Player->Action();
		return;
	}
}

void ACPlayerController::SkillPressed(ESkillButtonType InButton)
{
	CheckTrue(Player->GetRecipient()->GetIsWidgetOpen());

	ACSkill* waitSkill = Player->GetSkillWait();
	if (waitSkill != NULL)
	{
		CheckFalse(State->IsIdle());
		if (InButton == waitSkill->GetSkillButton())
		{
			FHitResult Hit;
			TArray<TEnumAsByte<EObjectTypeQuery>> objectType;
			objectType.Add(ObjectTypeQuery1);
			GetHitResultUnderCursorForObjects(objectType, false, Hit);
			if (Hit.bBlockingHit)
			{
				Player->SetTargetPosition(Hit.ImpactPoint);
				Player->Action();
			}
			else
				Player->SkillCancel();
		}
		else
			Player->SetSkillButton(InButton);
		return;
	}
	else
		Player->SetSkillButton(InButton);

	CheckFalse(State->IsIdle());
	if (Player->GetSKillIsHolding(InButton))
	{
		FHitResult Hit;
		TArray<TEnumAsByte<EObjectTypeQuery>> objectType;
		objectType.Add(ObjectTypeQuery1);
		GetHitResultUnderCursorForObjects(objectType, false, Hit);
		if (Hit.bBlockingHit)
		{
			Player->SetTargetPosition(Hit.ImpactPoint);
			Player->Action();
			return;
		}
	}
	if (Player->GetSKillIsImmediately(InButton))
	{
		
		FHitResult Hit;
		TArray<TEnumAsByte<EObjectTypeQuery>> objectType;
		objectType.Add(ObjectTypeQuery1);
		GetHitResultUnderCursorForObjects(objectType, false, Hit);
		if (Hit.bBlockingHit)
		{
			Player->SetTargetPosition(Hit.ImpactPoint);
			Player->Action();
		}

		return;
	}
}

void ACPlayerController::InteractionPressed()
{
	if (Player->GetRecipient()->GetIsWidgetOpen() == true)
	{
		DialogueUI->IfPressG();
		return;
	}
	ACNpc* npc = Player->GetRecipient()->GetNpc();
	if (npc != NULL)
	{
		Player->GetCharacterMovement()->StopMovementImmediately();
		FRotator rot = CHelpers::YawRotator(UKismetMathLibrary::FindLookAtRotation(Player->GetActorLocation(), npc->GetActorLocation()));
		Player->SetActorRotation(rot.Quaternion());
		DialogueUI->SetNpc(npc);
		npc->CameraMoving();
		DialogueUI->SetVisibility(ESlateVisibility::Visible);
		Player->GetRecipient()->SetIsWidgetOpen(true);
		PlayerUI->SetVisibility(ESlateVisibility::Collapsed);

		return;
	}
	ACInteractionObject* object = Player->GetRecipient()->GetInteractionObject();
	if (object != NULL)
	{
		object->Interaction();
		return;
	}
	if (Player->CanActionMoveInteraction())
	{
		Player->StartMoveInteraction();
		return;
	}
}

void ACPlayerController::OnQSkillReleased()
{
	Player->SetSkillReleased(ESkillButtonType::Q);
}

void ACPlayerController::OnWSkillReleased()
{
	Player->SetSkillReleased(ESkillButtonType::W);
}

void ACPlayerController::OnESkillReleased()
{
	Player->SetSkillReleased(ESkillButtonType::E);
}

void ACPlayerController::OnRSkillReleased()
{
	Player->SetSkillReleased(ESkillButtonType::R);
}

void ACPlayerController::OnASkillReleased()
{
	Player->SetSkillReleased(ESkillButtonType::A);
}

void ACPlayerController::OnSSkillReleased()
{
	Player->SetSkillReleased(ESkillButtonType::S);
}

void ACPlayerController::OnDSkillReleased()
{
	Player->SetSkillReleased(ESkillButtonType::D);
}

void ACPlayerController::OnFSkillReleased()
{
	Player->SetSkillReleased(ESkillButtonType::F);
}

void ACPlayerController::OnZSkillReleased()
{
}

void ACPlayerController::OnDashSkillReleased()
{
	Player->SetSkillReleased(ESkillButtonType::Dash);
}

void ACPlayerController::OnSetHoldingCameraPressed()
{
	CheckTrue(Player->GetRecipient()->GetIsWidgetOpen());
	Player->SetCameraMode(true);
}

void ACPlayerController::OnSetHoldingCameraReleased()
{
	CheckTrue(Player->GetRecipient()->GetIsWidgetOpen());
	Player->SetCameraMode(false);
}

void ACPlayerController::ZoomInOut(float Axis)
{
	CheckTrue(Player->GetRecipient()->GetIsWidgetOpen());
	Player->SetCameraZoom(Axis);
}
