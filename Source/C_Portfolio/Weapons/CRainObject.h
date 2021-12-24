#pragma once

#include "CoreMinimal.h"
#include "Weapons/CAttackObject.h"
#include "CRainObject.generated.h"

UCLASS()
class C_PORTFOLIO_API ACRainObject : public ACAttackObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
		float TickTime;

	UPROPERTY(EditAnywhere)
		float LifeTime;

	UPROPERTY(EditAnywhere)
		float Power;

	UPROPERTY(EditDefaultsOnly)
		class USoundBase* PlaneSound;

	UPROPERTY(EditDefaultsOnly)
		float Volume;
public:
	ACRainObject();


protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
		void EndOverlap(AActor* InOtherActor);

	UFUNCTION(BlueprintCallable)
		void SetHitActors(AActor* InActor);

	void Reset();

private:
	float CountDown;
};
