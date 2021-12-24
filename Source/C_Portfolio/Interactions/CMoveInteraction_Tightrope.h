#pragma once

#include "CoreMinimal.h"
#include "Interactions/CMoveInteraction.h"
#include "CMoveInteraction_Tightrope.generated.h"

UCLASS()
class C_PORTFOLIO_API ACMoveInteraction_Tightrope : public ACMoveInteraction
{
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly)
		class UAnimMontage* TightropeMontage;

	UPROPERTY(EditAnywhere)
		float PlayRatio;

	UPROPERTY(EditDefaultsOnly)
		FName StartSection;

	UPROPERTY(EditAnywhere)
		float MoveSpeed;

	UPROPERTY(EditDefaultsOnly)
		float RemainDist = 60.0f;

public:
	virtual void Start(class ACPlayer* InPlayer);
	virtual void Activity(ACPlayer* InPlayer);
	virtual void End(ACPlayer* InPlayer);

};
