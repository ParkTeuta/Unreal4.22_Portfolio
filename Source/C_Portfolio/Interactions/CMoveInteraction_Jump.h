#pragma once

#include "CoreMinimal.h"
#include "Interactions/CMoveInteraction.h"
#include "CMoveInteraction_Jump.generated.h"

UCLASS()
class C_PORTFOLIO_API ACMoveInteraction_Jump : public ACMoveInteraction
{
	GENERATED_BODY()
private:
	UPROPERTY(EditDefaultsOnly, Category = "Short")
		class UAnimMontage* ShortJumpMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Short")
		float ShortPlayRatio;

	UPROPERTY(EditDefaultsOnly, Category = "Short")
		FName ShortStartSection;
	
	UPROPERTY(EditDefaultsOnly, Category = "Long")
		class UAnimMontage* LongJumpMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Long")
		float LongPlayRatio;

	UPROPERTY(EditDefaultsOnly, Category = "Long")
		FName LongStartSection;

	UPROPERTY(EditAnywhere, Category = "Common")
		float JumpPower;

	UPROPERTY(EditDefaultsOnly, Category = "Common")
		float JumpRange;

public:
	virtual void Start(class ACPlayer* InPlayer);
	virtual void Activity(ACPlayer* InPlayer);
	virtual void End(ACPlayer* InPlayer);
	virtual void NotifyAction(ACPlayer* InPlayer);

private:
	void CheckAction();

	bool IsLong;
};
