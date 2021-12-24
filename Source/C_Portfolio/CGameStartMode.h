// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CGameStartMode.generated.h"

/**
 * 
 */
UCLASS()
class C_PORTFOLIO_API ACGameStartMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;
	ACGameStartMode();

private:
	TSubclassOf<UUserWidget> StartLevelWidgetClass;
	class UUCWidget_StartGame* StartLevelWidget;
};
