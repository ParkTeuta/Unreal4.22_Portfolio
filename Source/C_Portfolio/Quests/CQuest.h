
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/CQuestComponent.h"
#include "CQuest.generated.h"

UCLASS()
class C_PORTFOLIO_API ACQuest : public AActor
{
	GENERATED_BODY()

public:
	ACQuest();

public:
	//UPROPERTY(EditDefaultsOnly, Category = "Script")


public:
	UPROPERTY(EditDefaultsOnly)
		class UCScriptComponent* Script;

	UPROPERTY(EditAnywhere)
		TSubclassOf<ACQuest> QuestCondition;

	UPROPERTY(EditAnywhere)
		TSubclassOf<ACQuest> QuestCancelCondition;

	UPROPERTY(EditAnywhere)
		FString QuestName; 

	UPROPERTY(VisibleDefaultsOnly)
		class UCClearConditionComponent* ClearCondition;

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	virtual bool CheckQeustCondition() {  return false; }
	//virtual bool 

	//virtual FString GetNextScript() { return FString(); };
	virtual void CheckCondition(); //Tick에 호출, 실시간으로 퀘스트 받을 수 있는지 체크. 여기에 퀘스트 조건 작성
	virtual void CheckCancelCondition(); //Tick에 호출, 실시간으로 퀘스트 받을 수 있는지 체크. 여기에 퀘스트 조건 작성
	UFUNCTION(BlueprintPure)
		bool CheckClearCondition();

	UFUNCTION(BlueprintPure)
		FORCEINLINE FString GetQuestName() { return QuestName; }

	UFUNCTION(BlueprintNativeEvent, BlueprintPure)
		FString GetQuestContent();
		FString GetQuestContent_Implementation();

	UFUNCTION(BlueprintPure)
		TArray<FString> GetDialogue();

	UFUNCTION(BlueprintPure)
		TArray<FString> GetDialDatas(EQuestClearState InState);

	UFUNCTION(BlueprintPure)
		FMontageData_Npc GetDialMontage();

	UFUNCTION(BlueprintPure)
		TArray<FMontageData_Npc> GetDialMontages();

	UFUNCTION(BlueprintPure)
		FMontageData_Npc GetMontageDataToClearState(EQuestClearState InState);

	UFUNCTION(BlueprintPure)
		TArray<FMontageData_Npc> GetMontageDatasToClearState(EQuestClearState InState);

	//UFUNCTION(BlueprintPure)
	bool AddCount(AActor* InActor, int32 InCount);

	FString GetNoneDialogue();
	FMontageData_Npc GetNoneMontage();
	TArray<FMontageData_Npc> GetNoneMontages();

	UFUNCTION(BlueprintPure)
		bool GetIsConditionMet();

	FORCEINLINE void SetOwnerNpc(class ACNpc* InNPC) { OwnerNpc = InNPC; }
	FORCEINLINE class ACNpc* GetOwnerNpc() { return OwnerNpc; }
	FORCEINLINE void SetPlayer(class ACPlayer* InPlayer) { Player = InPlayer; }

	UFUNCTION(BlueprintPure)
		FORCEINLINE EQuestClearState GetClearState() { return ClearState; }

	UFUNCTION(BlueprintCallable)
		void SetClearState(EQuestClearState InState);

	UFUNCTION(BlueprintPure)
		int32 GetCount();
		
	void OffInteractionObjects();

public:
	//대화 마친 후 처리할 부분 함수들
	void EndDialogue();
	void SelectQuest();
	void QuestAccept();
	UFUNCTION(BlueprintNativeEvent)
		void QuestAcceptExtraActive();
		void QuestAcceptExtraActive_Implementation();

	void QuestReject();
	void QuestClear();

	UFUNCTION(BlueprintNativeEvent)
		void QuestClearExtraActive();
		void QuestClearExtraActive_Implementation();

	UFUNCTION(BlueprintImplementableEvent)
		EQuestClearState GetQuestConditionState(TSubclassOf<ACQuest> FindQuest);

	UFUNCTION(BlueprintCallable)
		void LoadQuest(EQuestClearState InState, int32 InCount);

protected:
	int32 QuestNumber;

	bool IsClear;

	bool bCantProgress;

	EQuestClearState ClearState;

	UPROPERTY(BlueprintReadOnly)
		class ACNpc* OwnerNpc;
	UPROPERTY(BlueprintReadOnly)
		class ACPlayer* Player;
};
