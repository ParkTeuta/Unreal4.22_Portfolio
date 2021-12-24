#include "CStatusComponent.h"
#include "Global.h"

UCStatusComponent::UCStatusComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UCStatusComponent::BeginPlay()
{
	Health = MaxHealth;

	Super::BeginPlay();
}

void UCStatusComponent::AddHealth(float InValue)
{
	Health += InValue;

	Health = FMath::Clamp(Health, 0.0f, MaxHealth);
}

void UCStatusComponent::SubHealth(float InValue)
{
	Health -= InValue;

	Health = FMath::Clamp(Health, 0.0f, MaxHealth);
}

void UCStatusComponent::SetMoveMode()
{
	bCanMove = true;
}

void UCStatusComponent::SetStopMode()
{
	bCanMove = false;
}

void UCStatusComponent::LoadingCoolTime(float tick)
{
}

void UCStatusComponent::ResetStatus()
{
	Health = MaxHealth;
	Mp = MaxMp;
}
