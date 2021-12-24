#pragma once

#include "CoreMinimal.h"
#include "Weapons/CAttackObject.h"
#include "CThrowItem_Sword_Tempest.generated.h"

UCLASS()
class C_PORTFOLIO_API ACThrowItem_Sword_Tempest : public ACAttackObject
{
	GENERATED_BODY()
	
private:
	UPROPERTY(VisibleDefaultsOnly)
		class USphereComponent* SphereCollision;

	UPROPERTY(VisibleDefaultsOnly)
		class UProjectileMovementComponent* ProjectileComponent;

	UPROPERTY(EditDefaultsOnly)
		float MovementDistance;

public:	
	ACThrowItem_Sword_Tempest();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

public:

	FORCEINLINE void SetTarget(ACharacter* InCharacter) { target = InCharacter; }

private:
	ACharacter* target;

	FVector StartPos;
};
