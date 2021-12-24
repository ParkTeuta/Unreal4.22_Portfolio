#pragma once

#include "CoreMinimal.h"
#include "Abilities/CAbility_Player.h"
#include "CAbility_Magic_WaterBoom.generated.h"

UCLASS()
class C_PORTFOLIO_API ACAbility_Magic_WaterBoom : public ACAbility_Player
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class ACThrow_WaterBoom> ThrowItem;

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<AActor> MagicCircleClass;
		//class ACThrowItem* ThrowItem;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

public:
	virtual void Behavior() override;
	virtual void Begin_Behavior() override;
	virtual void End_Behavior() override;



	virtual void SkillPreview() override;

	virtual void Cancel() override;
	//virtual float GetPower() override;

public:
	FORCEINLINE bool HitActorsOverCount() { return HitActors.Num() == Count; }

private:
	AActor* MagicCircle;
};
