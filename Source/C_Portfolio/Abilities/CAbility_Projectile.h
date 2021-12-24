#pragma once

#include "CoreMinimal.h"
#include "Abilities/CAbility_Player.h"
#include "CAbility_Projectile.generated.h"

UCLASS()
class C_PORTFOLIO_API ACAbility_Projectile : public ACAbility_Player
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class ACThrowItem> ThrowItem;
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

};
