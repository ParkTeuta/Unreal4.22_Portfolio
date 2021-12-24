#pragma once

#include "CoreMinimal.h"
#include "CAttackObject.h"
#include "Components/CMontagesComponent.h"
#include "CGenesisAttack.generated.h"

UCLASS()
class C_PORTFOLIO_API ACGenesisAttack : public ACAttackObject
{
	GENERATED_BODY()
	

public:	
	ACGenesisAttack();

	//void SetPosition();
	void GetShapeComponent(TArray<class UShapeComponent*>& OutComponents);

	UPROPERTY(EditAnywhere)
		float KunckBackPower;

public:
	UPROPERTY(VisibleDefaultsOnly)
		class UCMontagesComponent* Montages;

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
		void SetHitActors(AActor* InActor);

	UFUNCTION(BlueprintCallable)
		void EndOverlap(AActor* InOtherActor);

	UFUNCTION(BlueprintCallable)
		void Knockback();

public:
	UFUNCTION(BlueprintCallable, BlueprintPure)
		FORCEINLINE float GetRadius() { return Radius; }

protected:
	float Radius;

private:

};
