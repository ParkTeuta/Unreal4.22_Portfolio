#pragma once

#include "CoreMinimal.h"
#include "Weapons/CAttackObject.h"
#include "CBossSkill3_Middle_Object.generated.h"

UCLASS()
class C_PORTFOLIO_API ACBossSkill3_Middle_Object : public ACAttackObject
{
	GENERATED_BODY()


public:
	UPROPERTY(EditAnywhere)
		float AirbornePower;

private:
	UPROPERTY(VisibleDefaultsOnly)
		class USphereComponent* SphereCollision;

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class UParticleSystem> HitParticleClass;

public:
	ACBossSkill3_Middle_Object();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
		void SetHitActors(AActor* InActor);

	UFUNCTION(BlueprintCallable)
		void EndOverlap(AActor* InOtherActor);

	void Reset();
	void Action();

private:
	class UParticleSystem* HitParticle;

	float Power;
};
