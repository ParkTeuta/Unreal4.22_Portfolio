#pragma once

#include "CoreMinimal.h"
#include "Weapons/CAttackObject.h"
#include "CBossSkill1_Middle_Object.generated.h"

UCLASS()
class C_PORTFOLIO_API ACBossSkill1_Middle_Object : public ACAttackObject
{
	GENERATED_BODY()

public:
	ACBossSkill1_Middle_Object();
	void Action();

protected:
	virtual void BeginPlay() override;


	UFUNCTION(BlueprintCallable)
		void SetHitActors(AActor* InActor);

	UFUNCTION(BlueprintCallable)
		void EndOverlap(AActor* InOtherActor);


public:

	void GetShapeComponent(TArray<class UShapeComponent*>& OutComponents);

private:
	float Power;
};
