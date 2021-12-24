#include "CLevelTransferVolume.h"
#include "Global.h"
#include "Engine/Classes/Components/BoxComponent.h"

#include "Characters/CPlayer.h"

#include "Kismet/GameplayStatics.h"


ACLevelTransferVolume::ACLevelTransferVolume()
{
	PrimaryActorTick.bCanEverTick = true;

	TransferVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("TransferVolume"));
	RootComponent = TransferVolume;
	TransferVolume->SetCollisionProfileName(TEXT("OverlapOnlyPawn"));

}

void ACLevelTransferVolume::BeginPlay()
{
	Super::BeginPlay();
	
}

void ACLevelTransferVolume::NotifyActorBeginOverlap(AActor * OtherActor)
{

	ACPlayer* Player = Cast<ACPlayer>(OtherActor);
	if (Player != nullptr)
	{
		SaveState();
		UGameplayStatics::OpenLevel(this, *TransferLevelName);
	}
}

void ACLevelTransferVolume::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

