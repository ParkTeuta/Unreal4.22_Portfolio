#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/DataTable.h"
#include "CQuestComponent.h"
#include "CScriptComponent.generated.h"


//=============================================================================

USTRUCT(BlueprintType)
struct FScriptData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		EQuestClearState ClearStateType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FString Dialogue;

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
class C_PORTFOLIO_API UCScriptComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCScriptComponent();

private:
	UPROPERTY(EditAnywhere, Category = "DataTable")
		class UDataTable* DataTable;

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintPure)
		TArray<FString> GetDialogueDatas(EQuestClearState InState);

	UFUNCTION(BlueprintPure)
		FString GetDialogue(EQuestClearState InState);

	UFUNCTION(BlueprintPure)
		FMontageData_Npc GetDialMontage(EQuestClearState InState);

	UFUNCTION(BlueprintPure)
		TArray<FMontageData_Npc> GetDialMontages(EQuestClearState InState);

	UFUNCTION(BlueprintPure)
		FMontageData_Npc GetMontageToScriptData(TArray<FScriptData> InDial);

	UFUNCTION(BlueprintPure)
		TArray<FMontageData_Npc> GetMontageToScriptDatas(TArray<FScriptData> InDial);

	//UFUNCTION(BlueprintCallable)
		FORCEINLINE void EndDialogue() { DialCount = 0; }

private:
	TArray<FString> GetDialDatas(TArray<FScriptData> InDial);

	FString	 GetNoneDialData();
	FString	 GetBeforeDialData();
	FString	 GetAcceptDialData();
	FString	 GetRejectDialData();
	FString	 GetProceedingDialData();
	FString	 GetCompleteDialData();

	void AddNoneDialData(const FScriptData* InData);
	void AddBeforeDialData(const FScriptData* InData);
	void AddAcceptDialData(const FScriptData* InData);
	void AddRejectDialData(const FScriptData* InData);
	void AddProceedingDialData(const FScriptData* InData);
	void AddCompleteDialData(const FScriptData* InData);

private:
	int32 DialCount;

	TArray<FScriptData *> DialogueDatas;

	TArray<FScriptData> NoneDialogue;
	TArray<FScriptData> BeforeDialogue;
	TArray<FScriptData> AcceptDialogue;
	TArray<FScriptData> RejectDialogue;
	TArray<FScriptData> ProceedingDialogue;
	TArray<FScriptData> CompleteDialogue;
};
