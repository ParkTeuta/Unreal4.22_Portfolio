#pragma once

#include "Interface/CQuestInterface.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CInteractionObject.generated.h"

UCLASS()
class C_PORTFOLIO_API ACInteractionObject : public AActor, public ICQuestInterface
{
	GENERATED_BODY()
	
public:	
	ACInteractionObject();

public:
	UPROPERTY(EditAnywhere)
		FString ObjectName;

	UPROPERTY(EditAnywhere)
		float ActivationTime;

	UPROPERTY(EditAnywhere)
		int32 ItemCount;


	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
		class UStaticMeshComponent* ObjectMesh;

	UPROPERTY(VisibleDefaultsOnly)
		class UWidgetComponent* ActiveWidget;


protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
		virtual void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION(BlueprintCallable)
		virtual void OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Get Object Name")
		FString GetObjectName();
		virtual FString GetObjectName_Implementation() override;

	UFUNCTION(BlueprintCallable)
		virtual void Interaction() {};

	virtual void SetInteraction();
	virtual void EndInteraction() {};

	UFUNCTION(BlueprintCallable)
		void AddQuestItem(int32 InCount = 1);

	UFUNCTION(BlueprintPure)
		FORCEINLINE bool GetIsActive() { return IsActive; }

	bool CheckCanInteraction();

	void Activate_Object();
	void Deactivate_Object();
	void DeactivateAll();

	virtual void Reset() {};
	virtual void Cancel() {};


	FORCEINLINE void SetbQuestOn(bool InBoolean) { bQuestOn = InBoolean; }
	FORCEINLINE bool GetbQuestOn() { return bQuestOn; }

protected:
	class ACPlayer* Player;

	FTimerHandle timer;
//
//public:
//	UFUNCTION(BlueprintPure)
//		FORCEINLINE bool CanPreeG() { return IsActive && bQuestOn; }
//	
protected:
	bool IsActive;
	bool bQuestOn;

};
