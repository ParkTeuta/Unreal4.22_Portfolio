#pragma once

#include "CoreMinimal.h"
#include "CAttackObject.h"
#include "CEnemyThrowItem.generated.h"

UCLASS()
class C_PORTFOLIO_API ACEnemyThrowItem : public ACAttackObject
{
	GENERATED_BODY()
	
private:
	UPROPERTY(VisibleDefaultsOnly)
		class USphereComponent* SphereCollision;

public:
	UPROPERTY(VisibleDefaultsOnly)
		class UProjectileMovementComponent* ProjectileComponent;

public:
	void BeginOverlapToTarget(AActor* InOtherActor, AActor* InTargetActor, UParticleSystem* InParticle, FVector& InLocation);

public:	
	ACEnemyThrowItem();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual bool ExceptionCharacter(class ACharacter* InCharacter) override;
public:

	FORCEINLINE void SetTarget(ACharacter* InCharacter) { target = InCharacter; }

private:
	ACharacter* target;
};
