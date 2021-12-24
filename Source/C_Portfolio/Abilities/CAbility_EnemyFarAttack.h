#pragma once

#include "CoreMinimal.h"
#include "Abilities/CAbility.h"
#include "CAbility_EnemyFarAttack.generated.h"

UCLASS()
class C_PORTFOLIO_API ACAbility_EnemyFarAttack : public ACAbility
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class ACEnemyThrowItem> ThrowItem;
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
	virtual bool ExceptionCharacter(class ACharacter* InCharacter) override;
	
public:
	FORCEINLINE bool HitActorsOverCount() { return HitActors.Num() == Count; }

private:

};
