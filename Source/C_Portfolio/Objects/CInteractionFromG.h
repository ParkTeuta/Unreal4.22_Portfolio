#pragma once

#include "CoreMinimal.h"
#include "Objects/CInteractionObject.h"
#include "CInteractionFromG.generated.h"

UCLASS()
class C_PORTFOLIO_API ACInteractionFromG : public ACInteractionObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		class UAnimMontage* InteractionMontage;

	UPROPERTY(EditAnywhere)
		float NeedTime;

	UPROPERTY(EditAnywhere)
		FString BarText;

public:
	ACInteractionFromG();

	virtual void Tick(float DeltaTime) override;

public:
	//ACInteractionFromG();
	virtual void Interaction() override;

	UFUNCTION(BlueprintImplementableEvent)
		void WidgetOff();
	UFUNCTION(BlueprintImplementableEvent)
		void WidgetOn();

	virtual void EndInteraction() override;

	virtual void Cancel() override;
private:
	class UUCWidget_ElapsedBar* InteractionBar;
	float ElapsedTime;

};
