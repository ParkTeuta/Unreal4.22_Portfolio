#pragma once

#include "CoreMinimal.h"
#include "Abilities/CAbility_Enemy.h"
#include "CAbility_BossAttack.generated.h"

UCLASS()
class C_PORTFOLIO_API ACAbility_BossAttack : public ACAbility_Enemy
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

public:
	virtual void Behavior() override;
	virtual void Begin_Behavior() override;
	virtual void End_Behavior() override;

	virtual void OnColliders() override;
	virtual void OffColliders() override;

	virtual void SkillPreview() override;

	virtual void Cancel() override;
	//virtual float GetPower() override;

private:

};
