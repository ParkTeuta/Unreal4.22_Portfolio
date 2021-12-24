#pragma once

#include "CoreMinimal.h"
#include "CAttackObject.h"
#include "CThrow_KnockBack.generated.h"

UCLASS()
class C_PORTFOLIO_API ACThrow_KnockBack : public ACAttackObject
{
	GENERATED_BODY()
	

public:
	UPROPERTY(EditDefaultsOnly)
		float LifeTime;

	UPROPERTY(EditDefaultsOnly)
		float KnockBackPower;

private:
	UPROPERTY(VisibleDefaultsOnly)
		class USphereComponent* SphereCollision;

	UPROPERTY(VisibleDefaultsOnly)
		class UProjectileMovementComponent* ProjectileComponent;

public:	
	ACThrow_KnockBack();

protected:
	virtual void BeginPlay() override;

public:	


	UFUNCTION(BlueprintCallable)
		void EndOverlap(AActor* InOtherActor);

	void Reset();


private:
	float Power;
};
