#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CWidget_NpcHeadUI.generated.h"

UCLASS()
class C_PORTFOLIO_API UCWidget_NpcHeadUI : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent)
		void SetQuestOnOff(bool InOnOff);

	UFUNCTION(BlueprintImplementableEvent)
		void SetQuestClear();

	UFUNCTION(BlueprintImplementableEvent)
		void SetQuestProceeding();

	UFUNCTION(BlueprintImplementableEvent)
		void SetQuestReady();

	UFUNCTION(BlueprintImplementableEvent)
		void SetNPCName(const FString& InString);
};
