#pragma once

#include "CoreMinimal.h"
#include "CAttackObject.h"
#include "Components/CMontagesComponent.h"
#include "CRange.generated.h"

UCLASS()
class C_PORTFOLIO_API ACRange : public ACAttackObject
{
	GENERATED_BODY()
	

public:	
	ACRange();

	//void SetPosition();
	void GetShapeComponent(TArray<class UShapeComponent*>& OutComponents);

public:
	UPROPERTY(VisibleDefaultsOnly)
		class UCMontagesComponent* Montages;

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;


public:
	UFUNCTION(BlueprintCallable, BlueprintPure)
		FORCEINLINE float GetRadius() { return Radius; }

protected:
	float Radius;

private:

};
