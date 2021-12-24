#pragma once

#include "CoreMinimal.h"
#include "CAttackObject.h"
#include "CThrow_Void.generated.h"

UCLASS()
class C_PORTFOLIO_API ACThrow_Void : public ACAttackObject
{
	GENERATED_BODY()
	

public:
	UPROPERTY(EditAnywhere)
		float TickTime;

	UPROPERTY(EditAnywhere)
		float LifeTime;

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
		class USphereComponent* SphereCollision;

private:
	UPROPERTY(VisibleDefaultsOnly)
		class UProjectileMovementComponent* ProjectileComponent;


public:	
	ACThrow_Void();

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
	float Power;
};
