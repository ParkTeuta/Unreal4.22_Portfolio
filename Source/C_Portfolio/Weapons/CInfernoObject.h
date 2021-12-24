#pragma once

#include "CoreMinimal.h"
#include "Weapons/CAttackObject.h"
#include "CInfernoObject.generated.h"

UCLASS()
class C_PORTFOLIO_API ACInfernoObject : public ACAttackObject
{
	GENERATED_BODY()

public:
	ACInfernoObject();

	void OnCollision();

protected:
	virtual void BeginPlay() override;

public:

	void GetShapeComponent(TArray<class UShapeComponent*>& OutComponents);

private:
	float Power;
};
