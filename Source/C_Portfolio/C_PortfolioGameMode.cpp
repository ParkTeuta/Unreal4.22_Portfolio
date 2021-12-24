// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "C_PortfolioGameMode.h"
#include "C_PortfolioPlayerController.h"
#include "C_PortfolioCharacter.h"
#include "UObject/ConstructorHelpers.h"

AC_PortfolioGameMode::AC_PortfolioGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = AC_PortfolioPlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDownCPP/Blueprints/TopDownCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}