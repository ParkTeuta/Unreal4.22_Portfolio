#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UCWidget_ElapsedBar.generated.h"

UCLASS()
class C_PORTFOLIO_API UUCWidget_ElapsedBar : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintImplementableEvent)
		void UpdateElapsedBar(float NeedTime, float ElapsedTime);

	UFUNCTION(BlueprintImplementableEvent)
		void ResetElapseBar();

	UFUNCTION(BlueprintImplementableEvent)
		void SetBarText(const FString& InContent);
};
