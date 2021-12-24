#pragma once

#include "CoreMinimal.h"
#include "CAttackObject.h"
#include "CThrow_WaterBoom.generated.h"

UCLASS()
class C_PORTFOLIO_API ACThrow_WaterBoom : public ACAttackObject
{
	GENERATED_BODY()
	

public:
	UPROPERTY(EditDefaultsOnly)
		float AirbornePower;

	UPROPERTY(VisibleDefaultsOnly)
		class UProjectileMovementComponent* ProjectileComponent;

	UPROPERTY(EditDefaultsOnly)
		float Dist;

	UPROPERTY(EditDefaultsOnly)
		class USoundBase* PlaneSound;

	UPROPERTY(EditDefaultsOnly)
		float Volume;

public:
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
		class USphereComponent* SphereCollision;
	//UPROPERTY(VisibleDefaultsOnly)
	/*UPROPERTY(EditAnywhere)
		TArray<TSubclassOf<AActor>> PermitObject;*/
public:	
	ACThrow_WaterBoom();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

public:	
	UFUNCTION(BlueprintCallable)
		void EndOverlap(AActor* InOtherActor);

	UFUNCTION(BlueprintCallable)
		void SetHitActors(AActor* InActor);

	UFUNCTION(BlueprintCallable)
		void HitPlane();

	UFUNCTION(BlueprintImplementableEvent)
		void DestroyDecal();

	UFUNCTION(BlueprintImplementableEvent)
		void SetDecal(FVector InVector);
	void SetTargetPosition(FVector InVector);

	void Reset();


private:
	float Power;
	FVector TargetPosition;

};
