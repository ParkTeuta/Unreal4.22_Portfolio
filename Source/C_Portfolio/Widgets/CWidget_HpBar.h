#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CWidget_HpBar.generated.h"

UCLASS()
class C_PORTFOLIO_API UCWidget_HpBar : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent)
		void UpdateHealthBar(float InHealth, float InMaxHealth);

	UFUNCTION(BlueprintImplementableEvent)
		void UpdateName(const FString& InName);

};
