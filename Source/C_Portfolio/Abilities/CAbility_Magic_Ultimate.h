#pragma once

#include "CoreMinimal.h"
#include "Abilities/CAbility_Player.h"
#include "CAbility_Magic_Ultimate.generated.h"

UCLASS()
class C_PORTFOLIO_API ACAbility_Magic_Ultimate : public ACAbility_Player
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class AActor> HandAttackCircleClass;

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class AActor> GroundCircle1Class;

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class AActor> GroundCircle2Class;

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class ACRayAttack> RayClass;
		//class ACThrowItem* ThrowItem;
	UPROPERTY(EditDefaultsOnly)
		float RayLifeTime;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

public:
	virtual void Behavior() override;
	virtual void Begin_Behavior() override;
	virtual void Middle_Behavior() override;
	virtual void Special_Behavior() override;
	virtual void End_Behavior() override;



	virtual void SkillPreview() override;

	virtual void Cancel() override;
	//virtual float GetPower() override;

public:
	FORCEINLINE bool HitActorsOverCount() { return HitActors.Num() == Count; }

private:
	class ACRayAttack * Ray;

	AActor* HandAttachCircle;
	AActor* GroundCircle1;
	AActor* GroundCircle2;
};
