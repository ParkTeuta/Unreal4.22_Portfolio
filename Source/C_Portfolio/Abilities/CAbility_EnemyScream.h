#pragma once

#include "CoreMinimal.h"
#include "Abilities/CAbility_Enemy.h"
#include "CAbility_EnemyScream.generated.h"

UCLASS()
class C_PORTFOLIO_API ACAbility_EnemyScream : public ACAbility_Enemy
{
	GENERATED_BODY()

private:
	
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

public:
	virtual void Behavior() override;
	virtual void Begin_Behavior() override;

	UFUNCTION(BlueprintNativeEvent)
		void BeginBehaviorAction();
		void BeginBehaviorAction_Implementation();

	virtual void End_Behavior() override;


	virtual void Cancel() override;
	//virtual float GetPower() override;

private:

};
