// Fill out your copyright notice in the Description page of Project Settings.


#include "CGameStartMode.h"
#include "Global.h"
#include "Blueprint/UserWidget.h"
#include "Widgets/UCWidget_StartGame.h"

ACGameStartMode::ACGameStartMode()
{
	static ConstructorHelpers::FClassFinder<UUCWidget_StartGame> StartWidgetClass(TEXT("WidgetBlueprint'/Game/UI/W_StartLevel.W_StartLevel_C'"));

	if (StartWidgetClass.Succeeded())
		StartLevelWidgetClass = StartWidgetClass.Class;
}

void ACGameStartMode::BeginPlay()
{
	Super::BeginPlay();

	if (IsValid(StartLevelWidgetClass))
	{
		StartLevelWidget = Cast<UUCWidget_StartGame>(CreateWidget(GetWorld(), StartLevelWidgetClass));

		if (IsValid(StartLevelWidget))
		{
			StartLevelWidget->AddToViewport();
		}
	}
}