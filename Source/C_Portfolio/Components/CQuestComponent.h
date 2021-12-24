// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/DataTable.h"
#include "CQuestComponent.generated.h"


//=============================================================================

UENUM(BlueprintType)
enum class EQuestClearState : uint8
{
	Q_None, Q_BeforeProgress, Q_Accept, Q_Reject, Q_Proceeding, Q_Complete,
};

//=============================================================================

USTRUCT(BlueprintType)
struct FMontageData_Npc : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		class UAnimMontage* AnimMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float PlayRatio = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FName StartSection;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		bool bCanMove;
};

//=============================================================================



UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class C_PORTFOLIO_API UCQuestComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCQuestComponent();

public:
	UPROPERTY(EditAnywhere, Category = "Quest")
		TArray<TSubclassOf<class ACQuest>> ACQuestClass;

public:
	TArray<ACQuest *> GetQuestList();
	TArray<ACQuest *> GetQuestBeforeProgressList();
	TArray<ACQuest *> GetQuestProceedingList();
	TArray<ACQuest *> GetCanClearList();

	TArray<FString> GetErrorDialogue();

	FString GetNoneDialogue();

	TArray<FString> GetNoneDialDatas();

	FMontageData_Npc GetNoneMontage();
	TArray<FMontageData_Npc> GetNoneMontages();



protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


private:

	TArray<class ACQuest*> Quest;
	class ACPlayer* Player;

};
