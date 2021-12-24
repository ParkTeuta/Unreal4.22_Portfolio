#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UCWidget_StartGame.generated.h"

UCLASS()
class C_PORTFOLIO_API UUCWidget_StartGame : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;


private:
	UFUNCTION(BlueprintCallable)
		void StartButtonCallback();

	UFUNCTION(BlueprintCallable)
		void EndButtonCallback();

protected:
	UPROPERTY(BlueprintReadWrite, VisibleDefaultsOnly, meta = (AllowPrivateAccess = true))
		class UButton* StartButton;

	UPROPERTY(BlueprintReadWrite, VisibleDefaultsOnly, meta = (AllowPrivateAccess = true))
		class UButton* EndButton;
};
