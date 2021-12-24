#pragma once

#include "CoreMinimal.h"
#include "Weapons/CAttackObject.h"
#include "CPowerShotObject.generated.h"

UCLASS()
class C_PORTFOLIO_API ACPowerShotObject : public ACAttackObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
		float Power;

	UPROPERTY(EditDefaultsOnly)
		float SkillRangeDegree;

public:
	ACPowerShotObject();


protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
		void EndOverlap(AActor* InOtherActor);

	UFUNCTION(BlueprintCallable)
		void SetHitActors(AActor* InActor);

	UFUNCTION(BlueprintPure)
		FVector GetRightLeftVector(bool IsRight = true);

	void Action();

private:
};
