#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/CSkillComponent.h"
#include "CWidget_PlayerUI.generated.h"

UCLASS()
class C_PORTFOLIO_API UCWidget_PlayerUI : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent)
		void UpdateSkillSprite();

	UFUNCTION(BlueprintImplementableEvent)
		void UpdateSkillSpriteInSkillButton(ESkillButtonType InSkillButton, class UPaperSprite* InSprite);

	UFUNCTION(BlueprintImplementableEvent)
		void UpdateHealthBar(float InHealth, float InMaxHealth);

	UFUNCTION(BlueprintImplementableEvent)
		void UpdateClassSprite(class UPaperSprite* InSprite);

public:
	UPROPERTY(BlueprintReadOnly)
		class ACPlayer* Player;

	FORCEINLINE void SetPlayer(class ACPlayer* InPlayer) { Player = InPlayer; }
};
