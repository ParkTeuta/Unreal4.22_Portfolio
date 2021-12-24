#pragma once

#include "CoreMinimal.h"
#include "Weapons/CAttackObject.h"
#include "CBossSkill1_Special_ThrowItem.generated.h"

UCLASS()
class C_PORTFOLIO_API ACBossSkill1_Special_ThrowItem : public ACAttackObject
{
	GENERATED_BODY()
	

public:
	UPROPERTY(EditAnywhere)
		float LifeTime;

	UPROPERTY(EditAnywhere)
		float AirbornePower;

private:
	UPROPERTY(VisibleDefaultsOnly)
		class USphereComponent* SphereCollision;

	UPROPERTY(VisibleDefaultsOnly)
		class UProjectileMovementComponent* ProjectileComponent;

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class UParticleSystem> HitParticleClass;

public:	
	ACBossSkill1_Special_ThrowItem();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
		void SetHitActors(AActor* InActor);

	void Reset();


private:
	class UParticleSystem* HitParticle;

	float Power;
};
