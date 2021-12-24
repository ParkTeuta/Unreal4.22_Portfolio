#include "CMoveInteractionComponent.h"
#include "Global.h"
#include "Characters/CPlayer.h"

#include "CStateComponent.h"

UCMoveInteractionComponent::UCMoveInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}


void UCMoveInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	State = CHelpers::GetComponent<UCStateComponent>(GetOwner());
	Player = Cast<ACPlayer>(GetOwner());
}


void UCMoveInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	CheckNull(MoveInteraction);

	if (bMove == true)
	{
		MoveInteraction->Activity(Player);
	}
	if (MoveInteraction->GetMoveState() == EMoveState::MS_Finish)
	{
		End();
	}

}

ACMoveInteraction * UCMoveInteractionComponent::GetMoveInteraction()
{
	return MoveInteraction;
}

void UCMoveInteractionComponent::SetMoveInteraction(ACMoveInteraction * InMoveInteraction)
{
	MoveInteraction = InMoveInteraction;
}

void UCMoveInteractionComponent::Start()
{
	CheckNull(MoveInteraction);
	CheckTrue(State->IsMoveInteraction());
	MoveInteraction->Start(Player);
	State->SetMoveInteractionMode();
	bMove = true;
}

void UCMoveInteractionComponent::End()
{
	CheckNull(MoveInteraction);
	bMove = false;
	MoveInteraction->End(Player);
	State->SetIdleMode();
}

bool UCMoveInteractionComponent::CanActionMoveInteraction()
{
	CheckNullResult(MoveInteraction, false);
	CheckFalseResult(State->IsIdle(), false);
	CheckTrueResult(bMove, false);
	return true;
}

