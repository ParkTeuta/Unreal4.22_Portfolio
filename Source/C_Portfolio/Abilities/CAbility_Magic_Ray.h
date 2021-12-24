#pragma once

#include "CoreMinimal.h"
#include "Abilities/CAbility_Player.h"
#include "CAbility_Magic_Ray.generated.h"

UCLASS()
class C_PORTFOLIO_API ACAbility_Magic_Ray : public ACAbility_Player
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class ACRayAttack> RayAttack;

	UPROPERTY(EditAnywhere)
		float HoldingTime;
		
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

public:

	virtual void Behavior() override;
	virtual void Begin_Behavior() override;
	virtual void End_Behavior() override;

	virtual void Cancel() override;

private:
	FTimerHandle timer;
	ACRayAttack* ray;
};
