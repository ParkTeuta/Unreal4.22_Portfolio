#pragma once

#include "CoreMinimal.h"
#include "Abilities/CAbility.h"
#include "CAbility_Enemy_HerakliosAttack.generated.h"

UCLASS()
class C_PORTFOLIO_API ACAbility_Enemy_HerakliosAttack : public ACAbility
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class ACEnemyThrowItem_Parabola> ThrowItem;

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class AActor> ItemClass;
		//class ACThrowItem* ThrowItem;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

public:
	virtual void Behavior() override;
	virtual void Begin_Behavior() override;
	virtual void Middle_Behavior() override;
	virtual void End_Behavior() override;



	virtual void SkillPreview() override;

	virtual void Cancel() override;
	//virtual float GetPower() override;
	virtual bool ExceptionCharacter(class ACharacter* InCharacter) override;
	
public:
	FORCEINLINE bool HitActorsOverCount() { return HitActors.Num() == Count; }

private:
	AActor* Item;
};
