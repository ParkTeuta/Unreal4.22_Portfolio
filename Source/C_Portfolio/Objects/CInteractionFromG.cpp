#include "CInteractionFromG.h"
#include "Global.h"
#include "TimerManager.h"
#include "Characters/CPlayer.h"
#include "Components/CRecipientComponent.h"

#include "Components/WidgetComponent.h"
#include "Widgets/UCWidget_ElapsedBar.h"

ACInteractionFromG::ACInteractionFromG()
{
	BarText = "상호 작용 중입니다.";
}

void ACInteractionFromG::Tick(float DeltaTime)
{
	CheckNull(Player);
	CheckNull(InteractionBar);
	if (Player->GetRecipient()->GetIsInteraction())
	{
		ElapsedTime += DeltaTime;

		//ElapsedTime = FMath::Clamp(ElapsedTime, 0.0f, NeedTime);

		InteractionBar->UpdateElapsedBar(NeedTime, ElapsedTime);
		/*if (NeedTime <= ElapsedTime)
			EndInteraction();*/
	}
}

void ACInteractionFromG::Interaction()
{
	CheckFalse(IsActive);
	CheckFalse(CheckCanInteraction());
	ElapsedTime = 0.0f;
	WidgetOff();
	
	//Player Location, Rotation
	Player->GetMovementComponent()->StopMovementImmediately();
	FRotator rot = CHelpers::YawRotator(UKismetMathLibrary::FindLookAtRotation(Player->GetActorLocation(), GetActorLocation()));
	Player->SetActorRotation(rot.Quaternion());
	
	Player->PlayAnimMontage(InteractionMontage, 1.0f, "Start");
	Player->GetRecipient()->SetIsInteraction(true);
	GetWorldTimerManager().SetTimer(timer, this, &ACInteractionFromG::EndInteraction, NeedTime);

	InteractionBar = Player->GetInteractionBar();
	if (InteractionBar != NULL)
		InteractionBar->SetBarText(BarText);
}

void ACInteractionFromG::EndInteraction()
{
	GetWorldTimerManager().ClearTimer(timer);
	Player->PlayAnimMontage(InteractionMontage, 1.0f, "End");
	Player->GetRecipient()->SetIsInteraction(false);
	ElapsedTime = 0.0f;
	SetInteraction();

	if (InteractionBar != NULL)
	{
		InteractionBar->ResetElapseBar();
		InteractionBar = NULL;
	}
}

void ACInteractionFromG::Cancel()
{
	WidgetOn();
	ElapsedTime = 0.0f;
	GetWorldTimerManager().ClearTimer(timer);
	Player->StopAnimMontage();
	Player->GetRecipient()->SetIsInteraction(false);

	if (InteractionBar != NULL)
	{
		InteractionBar->ResetElapseBar();
		InteractionBar = NULL;
	}
}
