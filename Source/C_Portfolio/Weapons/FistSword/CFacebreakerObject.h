#pragma once

#include "CoreMinimal.h"
#include "Weapons/CAttackObject.h"
#include "CFacebreakerObject.generated.h"

UCLASS()
class C_PORTFOLIO_API ACFacebreakerObject : public ACAttackObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
		float Power;

	UPROPERTY(EditAnywhere)
		float GrabPower;

public:
	ACFacebreakerObject();


protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
		void EndOverlap(AActor* InOtherActor);

	UFUNCTION(BlueprintCallable)
		void SetHitActors(AActor* InActor);

	UFUNCTION(BlueprintCallable)
		void StopActors(AActor* InActor);

	void Action();

private:
};
