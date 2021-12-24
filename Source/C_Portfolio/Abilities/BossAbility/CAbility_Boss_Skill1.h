#pragma once

#include "CoreMinimal.h"
#include "Abilities/CAbility_Enemy.h"
#include "CAbility_Boss_Skill1.generated.h"

UCLASS()
class C_PORTFOLIO_API ACAbility_Boss_Skill1 : public ACAbility_Enemy
{
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class ACBossSkill1_Middle_Object> MiddleObjectClass;

	UPROPERTY(EditAnywhere)
		float MiddleObjectForward;

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class ACBossSkill1_Special_ThrowItem> ThrowItem;
	
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

public:
	virtual void Behavior() override;
	virtual void Begin_Behavior() override;
	virtual void Middle_Behavior() override;
	virtual void Special_Behavior() override;

	virtual void End_Behavior() override;

	virtual void OnColliders() override;
	virtual void OffColliders() override;

	virtual void SkillPreview() override;

	virtual void Cancel() override;
	//virtual float GetPower() override;

public:
	ACAbility_Boss_Skill1();

private:

	class ACEnemy_Boss* OwnerBoss;
	class ACBossSkill1_Middle_Object* MiddleObject;
};
