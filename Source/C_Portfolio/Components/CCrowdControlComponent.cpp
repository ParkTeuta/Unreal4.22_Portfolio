#include "CCrowdControlComponent.h"

UCCrowdControlComponent::UCCrowdControlComponent()
{
}


void UCCrowdControlComponent::BeginPlay()
{
	Super::BeginPlay();
	
	for (int i = 0; i < (uint8)ECrowdControl::LastItem; i++)
	{
		CCTimeArray.Add(-1.0f);
	}
}

void UCCrowdControlComponent::SetStunMode(float InTime)
{
	CCTimeArray[(uint8)ECrowdControl::Stun] = InTime;
}

void UCCrowdControlComponent::SetAirborneMode()
{
	CCTimeArray[(uint8)ECrowdControl::Airborne] = 99.0f;
}

void UCCrowdControlComponent::SetDownMode(float InTime)
{
	CCTimeArray[(uint8)ECrowdControl::Down] = InTime;
}

void UCCrowdControlComponent::SetNoneCC()
{
	for (int i = 0; i < (uint8)ECrowdControl::LastItem; i++)
	{
		CCTimeArray[i] = -1.0f;
	}
}

void UCCrowdControlComponent::SetCCMode(ECrowdControl InNewCC, float InTime)
{
	switch (InNewCC)
	{
	case ECrowdControl::None:
	{
		SetNoneCC();
	}
		break;
	case ECrowdControl::Stun:
	{
		SetStunMode(InTime);
	}
		break;
	case ECrowdControl::Airborne:
	{
		SetAirborneMode();
	}
		break;
	case ECrowdControl::Down:
	{
		SetDownMode(InTime);
	}
		break;
	default:
		break;
	}
	CCTimeArray[(uint8)ECrowdControl::None] = CCTimeArray[(uint8)ECrowdControl::None] < InTime ? InTime : CCTimeArray[(uint8)ECrowdControl::None];
}

void UCCrowdControlComponent::CCTick(float DeltaTime)
{
	for (int i = 0; i < (uint8)ECrowdControl::LastItem; i++)
	{
		if (CCTimeArray[i] <= 0.0f)
		{
			CCTimeArray[i] = -1.0f;
			continue;
		}
		CCTimeArray[i] -= DeltaTime;

	}

}
