#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/CQuestComponent.h"
#include "CNpc.generated.h"

UENUM(BlueprintType)
enum class EDialogueState : uint8
{
	D_None, D_QuestList, D_End, D_Accept, D_SelectQuest, D_Clear, D_Wait
};


UCLASS()
class C_PORTFOLIO_API ACNpc : public ACharacter
{
	GENERATED_BODY()

public:
	ACNpc();
	//virtual void OnConstruction(const FTransform& Transform) override;

protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		FText NpcName;

private:
	/*UPROPERTY(VisibleDefaultsOnly)
		class UTextRenderComponent* NameText;
*/
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class UUserWidget> HeadUIClass;

	UPROPERTY(VisibleDefaultsOnly)
		class UWidgetComponent* HeadUI;

public:
	void SetHeadUIOnOff(bool InOnOff);
	void SetHeadUIQuestClear();
	void SetHeadUIQuestProceeding();
	void SetHeadUIQuestReady();
	void SetHeadUINpcName();

private:

	UPROPERTY(VisibleDefaultsOnly)
		class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleDefaultsOnly)
		class UCQuestComponent* QuestComp;

public://정리할겸
	UFUNCTION(BlueprintCallable)
		TArray<FString> DialProcessing(); //계속 로테이션 돌 프로세스

	UFUNCTION(BlueprintCallable)
		void PlayProcessingMontage();

	//quest골랐을때 행동할 거.
	UFUNCTION(BlueprintCallable)
		TArray<FString> SelectQuestByNum(int32 InNum); 

	UFUNCTION(BlueprintCallable)
		TArray<FString> SelectQuestByActor(class ACQuest* InQuest);

	UFUNCTION(BlueprintCallable)
		void PlayMontageByActor(class ACQuest* InQuest);

	UFUNCTION(BlueprintCallable)
		TArray<FString> SelectAccept();

	UFUNCTION(BlueprintCallable)
		TArray<FString> SelectReject();

	UFUNCTION(BlueprintCallable)
		void PlayAcceptMontage();

	UFUNCTION(BlueprintCallable)
		void PlayRejectMontage();

	UFUNCTION(BlueprintCallable)
		void PlayMontageByCount(int32 InCount);

public:
	UFUNCTION(BlueprintPure)
		TArray<class ACQuest *> GetQuestList(); //진행 가능, 진행 중 퀘스트 리스트, 

	UFUNCTION(BlueprintPure)
		TArray<class ACQuest *> GetQuestBeforeProgressList(); //진행전 리스트.

	UFUNCTION(BlueprintPure)
		TArray<class ACQuest *> GetQuestProceedingList(); //진행중 리스트.

	UFUNCTION(BlueprintPure)
		TArray<class ACQuest *> GetCanClearList(); //완료 가능한 것들.


	UFUNCTION(BlueprintPure)
		FString GetNoneDialogue();

	UFUNCTION(BlueprintPure)
		TArray<FString> GetNoneDialDatas();

	UFUNCTION(BlueprintPure)
		FMontageData_Npc GetNoneMontage();

	UFUNCTION(BlueprintPure)
		TArray<FMontageData_Npc> GetNoneMontages();

	UFUNCTION(BlueprintCallable)
		void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION(BlueprintCallable)
		void OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION(BlueprintPure)
		FORCEINLINE EDialogueState GetDialState() { return DialState; }

	UFUNCTION(BlueprintCallable)
		void EndDialogue();

	UFUNCTION(BlueprintCallable)
		void ResetDialogue();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		void CameraMoving();
		void CameraMoving_Implementation();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		void CameraReset();
		void CameraReset_Implementation();

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	FORCEINLINE void SetbHeadUIOnOff(bool InOnOff) { bHeadUIOnOff = InOnOff; }
	FORCEINLINE bool GetbHeadUIOnOff() { return bHeadUIOnOff; }

	class ACPlayer* GetPlayer();

private:
	EDialogueState DialState;

	class ACQuest* TempQuest;

	class ACPlayer* Player;

	FMontageData_Npc TempMontage;
	TArray<FMontageData_Npc> TempMontages;

	bool bHeadUIOnOff = false;
};
