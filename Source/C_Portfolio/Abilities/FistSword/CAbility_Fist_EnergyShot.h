#pragma once

#include "CoreMinimal.h"
#include "Abilities/CAbility_Player.h"
#include "CAbility_Fist_EnergyShot.generated.h"

UCLASS()
class C_PORTFOLIO_API ACAbility_Fist_EnergyShot : public ACAbility_Player
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class ACAttackObject> RayAttack;

	UPROPERTY(EditAnywhere)
		float HoldingTime;

	UPROPERTY(EditDefaultsOnly)
		float PowerMin;

	UPROPERTY(EditDefaultsOnly)
		float PowerMax;
		
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

protected:
	UFUNCTION(BlueprintImplementableEvent)
		void ZoomIn();
	UFUNCTION(BlueprintImplementableEvent)
		void ZoomInStop();

	UFUNCTION(BlueprintImplementableEvent)
		void ZoomOut();
	

public:

	virtual void Behavior() override;
	virtual void Begin_Behavior() override;
	virtual void Middle_Behavior() override;
	virtual void End_Behavior() override;

	virtual void Cancel() override;

	virtual float GetPower() override;

private:
	void FinishCharge();

	float CalculationPower();

private:
	FTimerHandle timer;
	ACAttackObject* Ray;

	float thisPower;

	float ChargingTime = 0.0f;
	bool IsCharging;
};
