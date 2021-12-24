#pragma once

#include "CoreMinimal.h"
#include "Abilities/CAbility.h"
#include "CAbility_Dummy.generated.h"

UCLASS()
class C_PORTFOLIO_API ACAbility_Dummy : public ACAbility
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
};
