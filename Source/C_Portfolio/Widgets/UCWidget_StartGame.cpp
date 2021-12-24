#include "UCWidget_StartGame.h"
#include "Global.h"
#include "Components/Button.h"

void UUCWidget_StartGame::NativeConstruct()
{
	Super::NativeConstruct();

	StartButton = Cast<UButton>(GetWidgetFromName(TEXT("Button_Start")));
	EndButton = Cast<UButton>(GetWidgetFromName(TEXT("Button_End")));

	StartButton->OnClicked.AddDynamic(this, &UUCWidget_StartGame::StartButtonCallback);
	EndButton->OnClicked.AddDynamic(this, &UUCWidget_StartGame::EndButtonCallback);
}

void UUCWidget_StartGame::StartButtonCallback()
{
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("MainMap3"));
}

void UUCWidget_StartGame::EndButtonCallback()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), GetWorld()->GetFirstPlayerController(), EQuitPreference::Quit, true);
}
