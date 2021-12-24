#pragma once

#include "CoreMinimal.h"
#include "Weapons/CAttackObject.h"
#include "CRayAttack.generated.h"

UCLASS()
class C_PORTFOLIO_API ACRayAttack : public ACAttackObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
		float TickTime;

	UPROPERTY(EditAnywhere)
		float Power;

	UPROPERTY(EditDefaultsOnly)
		class USoundBase* PlaneSound;

	UPROPERTY(EditDefaultsOnly)
		float Volume;

public:
	ACRayAttack();
	virtual void Tick(float DeltaTime) override;

	virtual void BeginOverlap(AActor* InOtherActor, UParticleSystem* InParticle, FVector& InLocation) override;

	UFUNCTION(BlueprintCallable)
		void EndOverlap(AActor* InOtherActor);

	UFUNCTION(BlueprintCallable)
		void SetHitActors(AActor* InActor);

	void Reset();

	void SetPower(float InPower);

private:
	float CountDown;
};
