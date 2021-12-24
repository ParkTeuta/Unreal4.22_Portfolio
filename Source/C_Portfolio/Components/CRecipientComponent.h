#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CRecipientComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class C_PORTFOLIO_API UCRecipientComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCRecipientComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintPure)
		TArray<class ACQuest*> GetQuestList();

	UFUNCTION(BlueprintPure)
		FORCEINLINE bool GetIsWidgetOpen() { return IsWidgetOpen; }

	UFUNCTION(BlueprintPure)
		class ACNpc* GetNpc();

	UFUNCTION(BlueprintPure)
		class ACInteractionObject* GetInteractionObject();

	UFUNCTION(BlueprintCallable)
		void SetIsWidgetOpen(bool InBoolean);

	UFUNCTION(BlueprintCallable)
		void AddCount(AActor* InActor, int32 InCount);
	
	/*UFUNCTION(BlueprintNativeEvent)
		void RefreshWidget(int32 InCount);
	void RefreshWidget_Implementation(int32 InCount);*/
	
	void RefreshQuestItem(int32 InCount);

	void SetObject(class ACInteractionObject* InObject);
	void SetNpc(ACNpc* InNpc);
	void AddQuestList(class ACQuest* InQuest);
	void DeleteQuest(class ACQuest* InQuest);

	//FORCEINLINE TArray<class ACQuest*> GetQuestClearList() { return QuestClearList; }
	//FORCEINLINE TArray<class ACQuest*> GetQuestList() { return QuestList; }

	//FORCEINLINE void SetQuestClearList(TArray<class ACQuest*> InList) { QuestClearList = InList; }
	void SetQuestList(TArray<class ACQuest*> InList);

	//FORCEINLINE TArray<class ACInteractionObject*> GetActiveObjectList() { return QuestList; }

	void QuestClear(class ACQuest* InQuest);

	FORCEINLINE bool GetIsInteraction() { return IsInteraction; }
	FORCEINLINE void SetIsInteraction(bool InIsInteraction) { IsInteraction = InIsInteraction; }

	void CancelInteraction();

	void AddActiveObject(class ACInteractionObject* InObject);
	bool CheckCanActive(class ACInteractionObject* InObject);
	void DeleteActiveObject(class ACInteractionObject* InObject);

private:
	bool IsWidgetOpen;
	bool IsInteraction;

	class ACInteractionObject* QuestObject;
	class ACNpc* Npc;
	TArray<class ACQuest*> QuestList;
	//TArray<class ACQuest*> QuestClearList;
	TArray<class ACInteractionObject*> ActiveObject;
};
