#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CCrowdControlComponent.generated.h"


UENUM(BlueprintType)
enum class ECrowdControl : uint8
{
	None, Stun, Airborne, Down, LastItem
}; //LastItem


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class C_PORTFOLIO_API UCCrowdControlComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintPure, BlueprintCallable)
		FORCEINLINE bool IsStun() { return CCTimeArray[(uint8)ECrowdControl::Stun] > 0.0f; }

	UFUNCTION(BlueprintPure, BlueprintCallable)
		FORCEINLINE bool IsAirborne() { return CCTimeArray[(uint8)ECrowdControl::Airborne] > 0.0f; }

	UFUNCTION(BlueprintPure, BlueprintCallable)
		FORCEINLINE bool IsDown() { return CCTimeArray[(uint8)ECrowdControl::Down] > 0.0f; }

	UFUNCTION(BlueprintPure, BlueprintCallable)
		FORCEINLINE bool IsNoneCC() { return CCTimeArray[(uint8)ECrowdControl::None] == -1.0f; }

	UFUNCTION(BlueprintPure, BlueprintCallable)
		FORCEINLINE bool IsCC() { return CCTimeArray[(uint8)ECrowdControl::None] != -1.0f;}

	FORCEINLINE float GetCCTime(ECrowdControl InNewCC) { return CCTimeArray[(uint8)InNewCC]; }
	//FORCEINLINE void SetCCTime(ECrowdControl InNewCC, float InTime) { CCTime = InTime; }

public:	
	UCCrowdControlComponent();

	void CCTick(float Delta);

	void SetCCMode(ECrowdControl InNewCC, float InTime = 0.0f);
	//CC
	void SetStunMode(float InTime);
	void SetAirborneMode();
	FORCEINLINE void ResetAirborne() { CCTimeArray[(uint8)ECrowdControl::Airborne] = -1.0f; }
	void SetDownMode(float InTime);
	void SetNoneCC();

protected:
	virtual void BeginPlay() override;
	//virtual void Tick(float DeltaTime) override;

private:
	TArray<float> CCTimeArray;
};
