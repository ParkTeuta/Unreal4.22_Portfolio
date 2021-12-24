#pragma once

#include "CoreMinimal.h"
#include "Weapons/CAttackObject.h"
#include "CJumpDownObject.generated.h"

UCLASS()
class C_PORTFOLIO_API ACJumpDownObject : public ACAttackObject
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleDefaultsOnly)
		class USphereComponent* SphereCollision;

	
public:
	ACJumpDownObject();

};
