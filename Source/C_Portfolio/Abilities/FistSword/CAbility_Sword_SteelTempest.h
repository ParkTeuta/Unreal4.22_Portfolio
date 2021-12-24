#pragma once

#include "CoreMinimal.h"
#include "Abilities/CAbility_Player.h"
#include "CAbility_Sword_SteelTempest.generated.h"

UCLASS()
class C_PORTFOLIO_API ACAbility_Sword_SteelTempest : public ACAbility_Player
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class ACThrowItem_Sword_Tempest> Tempest;

	UPROPERTY(EditDefaultsOnly)
		float StackDuration;
		//class ACThrowItem* ThrowItem;


protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

public:
	virtual void Behavior() override;
	virtual void Begin_Behavior() override;
	//virtual void Special_Behavior() override;
	virtual void End_Behavior() override;

	virtual void OnColliders() override;
	virtual void OffColliders() override;


	virtual void SkillPreview() override;

	virtual void Cancel() override;
	//virtual float GetPower() override;

public:
	FORCEINLINE bool HitActorsOverCount() { return HitActors.Num() == Count; }

private:


	int32 Stack;
	float StackTime;

};
