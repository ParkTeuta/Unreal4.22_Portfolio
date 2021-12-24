#pragma once

#include "CoreMinimal.h"
#include "Objects/CInteractionObject.h"
#include "CInteractionObjectFromAttack.generated.h"

UCLASS()
class C_PORTFOLIO_API ACInteractionObjectFromAttack : public ACInteractionObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
		int32 NeedHitCount;

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
		class UCapsuleComponent* Capsule;

public:
	//UFUNCTION(BlueprintImplementableEvent)
		void SetVisibililty(bool InBoolean);

	UFUNCTION(BlueprintImplementableEvent)
		void ShakeObject();

	UFUNCTION(BlueprintImplementableEvent)
		void SetFadeOut();

	UFUNCTION(BlueprintImplementableEvent)
		void SetFadeIn();

public:
	ACInteractionObjectFromAttack();

public:
	virtual void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult) override;

	virtual void Interaction() override;
	virtual void EndInteraction() override;

	virtual void Reset() override;
private:
	int32 HitCount;
};
