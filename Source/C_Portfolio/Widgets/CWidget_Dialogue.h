// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/CQuestComponent.h"
#include "CWidget_Dialogue.generated.h"

UCLASS()
class C_PORTFOLIO_API UCWidget_Dialogue : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite)
		class UButton* NextButton;

	UPROPERTY(BlueprintReadWrite)
		class UButton* CloseButton;

	UPROPERTY(BlueprintReadWrite)
		class UButton* AcceptButton;

	UPROPERTY(BlueprintReadWrite)
		class UButton* RejectButton;

	UPROPERTY(BlueprintReadWrite)
		class UTextBlock * NpcNameBlock;

	UPROPERTY(BlueprintReadWrite)
		class UMultiLineEditableTextBox* ContextBox;

	UPROPERTY(BlueprintReadWrite)
		class UListView* QuestList;

public:
	UFUNCTION(BlueprintCallable)
		void SetWidgets(UButton* InNextButton, UButton* InCloseButton, UButton* InAcceptButton, UButton* InRejectButton, UTextBlock * InNpcNameBlock, UMultiLineEditableTextBox* InContextBox, UListView* InQuestList);

public:

	UFUNCTION(BlueprintCallable)
		void OnNextButtonClicked();
	UFUNCTION(BlueprintCallable)
		void OnCloseButtonClicked();
	UFUNCTION(BlueprintCallable)
		void OnAcceptButtonClicked();
	UFUNCTION(BlueprintCallable)
		void OnRejectButtonClicked();

	UFUNCTION(BlueprintCallable)
		void OnQuestListItemClicekd(UObject* Item);


	void SetNpc(class ACNpc* InNpc);

	void NextScript();

	void Reset();

	void OnSelectButton();

	void OnQuestList();

	void EndButtonOn();

	void NextButtonOn();

	void RefreshListBox();

	void OnOffListBox(ESlateVisibility InVisibility);

	UFUNCTION(BlueprintCallable)
	void StartDialogue(TArray<FString> InDialDatas);

	void OffSelectButton();

	UFUNCTION(BlueprintCallable)
	void CheckDialEnd();

	void IfPressG();


protected:
	class ACQuest* TempQuest;

	UPROPERTY(BlueprintReadOnly)
		class ACNpc* Npc;

	TArray<FString> DialDatas;

	int32 Count;

	bool IsDialFinished;
	bool IsEnd;

};
