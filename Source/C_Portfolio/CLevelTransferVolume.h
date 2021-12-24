#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CLevelTransferVolume.generated.h"

UCLASS()
class C_PORTFOLIO_API ACLevelTransferVolume : public AActor
{
	GENERATED_BODY()
	
public:	
	ACLevelTransferVolume();

protected:
	virtual void BeginPlay() override;

protected:
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

	UFUNCTION(BlueprintImplementableEvent)
		void SaveState();

private:
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
		FString TransferLevelName;

	UPROPERTY()
		class UBoxComponent* TransferVolume;


public:	
	virtual void Tick(float DeltaTime) override;

};
