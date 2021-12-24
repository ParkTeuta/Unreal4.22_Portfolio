#pragma once

#include "CoreMinimal.h"
#include "CAttackObject.h"
#include "CThrowItem.generated.h"

UCLASS()
class C_PORTFOLIO_API ACThrowItem : public ACAttackObject
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
		class USphereComponent* SphereCollision;
private:

	UPROPERTY(VisibleDefaultsOnly)
		class UProjectileMovementComponent* ProjectileComponent;

public:
	void BeginOverlapToTarget(AActor* InOtherActor, AActor* InTargetActor, UParticleSystem* InParticle, FVector& InLocation);

public:	
	ACThrowItem();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

public:

	FORCEINLINE void SetTarget(ACharacter* InCharacter) { target = InCharacter; }

private:
	ACharacter* target;
};
