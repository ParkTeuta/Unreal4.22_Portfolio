#pragma once

#include "CoreMinimal.h"
#include "Weapons/CAttackObject.h"
#include "CFateSealedObject.generated.h"

UCLASS()
class C_PORTFOLIO_API ACFateSealedObject : public ACAttackObject
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleDefaultsOnly)
		class USphereComponent* SphereCollision;

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
		class UBoxComponent* BoxCollision;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float Range;

	UPROPERTY(EditAnywhere)
		float Power;

	UPROPERTY(EditAnywhere)
		float GrabPower;

public:
	ACFateSealedObject();


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
	void EndAction();

private:
};
