#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CWidget_Skill.generated.h"

UCLASS()
class C_PORTFOLIO_API UCWidget_Skill : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent)
		void UpdateSkillSprite();
};
