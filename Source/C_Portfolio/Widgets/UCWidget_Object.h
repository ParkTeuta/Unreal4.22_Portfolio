#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UCWidget_Object.generated.h"

UCLASS()
class C_PORTFOLIO_API UUCWidget_Object : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintImplementableEvent)
		void OnOffActive(bool InOnOff);
};
