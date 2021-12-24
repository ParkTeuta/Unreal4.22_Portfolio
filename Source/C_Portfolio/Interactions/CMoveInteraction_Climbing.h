#pragma once

#include "CoreMinimal.h"
#include "Interactions/CMoveInteraction.h"
#include "CMoveInteraction_Climbing.generated.h"

UCLASS()
class C_PORTFOLIO_API ACMoveInteraction_Climbing : public ACMoveInteraction
{
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly, Category = "Climbing")
		class UAnimMontage* ClimbingMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Climbing")
		float ClimbingPlayRatio;

	UPROPERTY(EditDefaultsOnly, Category = "Climbing")
		FName ClimbingStartSection;

	UPROPERTY(EditDefaultsOnly, Category = "Climbing")
		class UAnimMontage* ClimbingEndMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Climbing")
		float ClimbingEndPlayRatio;

	UPROPERTY(EditDefaultsOnly, Category = "Climbing")
		FName ClimbingEndStartSection;

	UPROPERTY(EditDefaultsOnly, Category = "JumpDown")
		class UAnimMontage* JumpDownMontage;

	UPROPERTY(EditDefaultsOnly, Category = "JumpDown")
		float JumpDownPlayRatio;

	UPROPERTY(EditDefaultsOnly, Category = "JumpDown")
		FName JumpDownStartSection;

	UPROPERTY(EditDefaultsOnly, Category = "Common")
		float MoveSpeed;

	UPROPERTY(EditDefaultsOnly, Category = "Common")
		float RemainDist = 60.0f;
	
	UPROPERTY(EditDefaultsOnly, Category = "Common")
		float ActinoChangeDist = 60.0f;

public:
	virtual void Start(class ACPlayer* InPlayer);
	virtual void Activity(ACPlayer* InPlayer);
	virtual void End(ACPlayer* InPlayer);
	virtual void NotifyAction(ACPlayer* InPlayer);

private:
	void CheckAction();
	bool IsClimbing;

	bool IsClimbingStop;
	bool IsTop;

	bool IsDown;
};
