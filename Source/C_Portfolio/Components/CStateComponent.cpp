#include "CStateComponent.h"

UCStateComponent::UCStateComponent()
{

}


void UCStateComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

void UCStateComponent::ChangeType(EStateType InNewType)
{
	EStateType prevType = Type;
	Type = InNewType;

	if (OnStateTypeChanged.IsBound())
		OnStateTypeChanged.Broadcast(prevType, Type);
}

void UCStateComponent::SetIdleMode()
{
	ChangeType(EStateType::Idle);
}

void UCStateComponent::SetDeadMode()
{
	ChangeType(EStateType::Dead);
}

void UCStateComponent::SetAttackMode()
{
	ChangeType(EStateType::Attack);
}

void UCStateComponent::SetSkillMode()
{
	ChangeType(EStateType::Skill);
}

void UCStateComponent::SetDashMode()
{
	ChangeType(EStateType::Dash);
}

void UCStateComponent::SetHittedMode()
{
	ChangeType(EStateType::Hitted);
}

void UCStateComponent::SetNoneMode()
{
	ChangeType(EStateType::None);
}

void UCStateComponent::SetEquippingMode()
{
	ChangeType(EStateType::Equipping);
}

void UCStateComponent::SetUnequippingMode()
{
	ChangeType(EStateType::Unequipping);
}

void UCStateComponent::SetMoveInteractionMode()
{
	ChangeType(EStateType::MoveInteraction);
}