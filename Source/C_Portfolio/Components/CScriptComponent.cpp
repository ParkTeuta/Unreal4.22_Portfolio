#include "CScriptComponent.h"
#include "Global.h"

#include "Animation/AnimMontage.h"

UCScriptComponent::UCScriptComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}


void UCScriptComponent::BeginPlay()
{
	Super::BeginPlay();

	Check(DataTable != NULL, "DataTable!=NULL");

	DataTable->GetAllRows<FScriptData>("", DialogueDatas);
	for (const FScriptData* data : DialogueDatas)
	{
		switch (data->ClearStateType)
		{
		case EQuestClearState::Q_None: AddNoneDialData(data); break;
		case EQuestClearState::Q_BeforeProgress: AddBeforeDialData(data); break;
		case EQuestClearState::Q_Accept: AddAcceptDialData(data); break;
		case EQuestClearState::Q_Reject: AddRejectDialData(data); break;
		case EQuestClearState::Q_Proceeding: AddProceedingDialData(data); break;
		case EQuestClearState::Q_Complete: AddCompleteDialData(data); break;
		}
		
	}
}


void UCScriptComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

TArray<FString> UCScriptComponent::GetDialogueDatas(EQuestClearState InState)
{
	switch (InState)
	{
	case EQuestClearState::Q_None: return  GetDialDatas(NoneDialogue); break;
	case EQuestClearState::Q_BeforeProgress: return GetDialDatas(BeforeDialogue); break;
	case EQuestClearState::Q_Accept: return GetDialDatas(AcceptDialogue); break;
	case EQuestClearState::Q_Reject: return GetDialDatas(RejectDialogue); break;
	case EQuestClearState::Q_Proceeding: return GetDialDatas(ProceedingDialogue); break;
	case EQuestClearState::Q_Complete: return GetDialDatas(CompleteDialogue);  break;
	}
	return TArray<FString>();
}

FString UCScriptComponent::GetDialogue(EQuestClearState InState)
{
	switch (InState)
	{
	case EQuestClearState::Q_None: return GetNoneDialData();  break;
	case EQuestClearState::Q_BeforeProgress: return GetBeforeDialData(); break;
	case EQuestClearState::Q_Accept: return GetAcceptDialData(); break;
	case EQuestClearState::Q_Reject: return GetRejectDialData(); break;
	case EQuestClearState::Q_Proceeding: return GetProceedingDialData(); break;
	case EQuestClearState::Q_Complete: return GetCompleteDialData();  break;
	}
	return FString();
}

FMontageData_Npc UCScriptComponent::GetDialMontage(EQuestClearState InState)
{
	switch (InState)
	{
	case EQuestClearState::Q_None: return  GetMontageToScriptData(NoneDialogue); break;
	case EQuestClearState::Q_BeforeProgress: return GetMontageToScriptData(BeforeDialogue); break;
	case EQuestClearState::Q_Accept: return GetMontageToScriptData(AcceptDialogue); break;
	case EQuestClearState::Q_Reject: return GetMontageToScriptData(RejectDialogue); break;
	case EQuestClearState::Q_Proceeding: return GetMontageToScriptData(ProceedingDialogue); break;
	case EQuestClearState::Q_Complete: return GetMontageToScriptData(CompleteDialogue);  break;
	}
	return FMontageData_Npc();
}

TArray<FMontageData_Npc> UCScriptComponent::GetDialMontages(EQuestClearState InState)
{
	switch (InState)
	{
	case EQuestClearState::Q_None: return  GetMontageToScriptDatas(NoneDialogue); break;
	case EQuestClearState::Q_BeforeProgress: return GetMontageToScriptDatas(BeforeDialogue); break;
	case EQuestClearState::Q_Accept: return GetMontageToScriptDatas(AcceptDialogue); break;
	case EQuestClearState::Q_Reject: return GetMontageToScriptDatas(RejectDialogue); break;
	case EQuestClearState::Q_Proceeding: return GetMontageToScriptDatas(ProceedingDialogue); break;
	case EQuestClearState::Q_Complete: return GetMontageToScriptDatas(CompleteDialogue);  break;
	}
	return TArray<FMontageData_Npc>();
}

FMontageData_Npc UCScriptComponent::GetMontageToScriptData(TArray<FScriptData> InDial)
{
	FMontageData_Npc montage = FMontageData_Npc();

	if (InDial.Num() > 0)
	{
		montage.AnimMontage = InDial[0].AnimMontage;
		montage.PlayRatio = InDial[0].PlayRatio;
		montage.StartSection = InDial[0].StartSection;
		montage.bCanMove = InDial[0].bCanMove;
	}
	return montage;
}

TArray<FMontageData_Npc> UCScriptComponent::GetMontageToScriptDatas(TArray<FScriptData> InDial)
{
	TArray<FMontageData_Npc> montages;
	for (FScriptData script : InDial)
	{
		FMontageData_Npc montage = FMontageData_Npc();
		montage.AnimMontage = script.AnimMontage;
		montage.PlayRatio = script.PlayRatio;
		montage.StartSection = script.StartSection;
		montage.bCanMove = script.bCanMove;
		montages.Add(montage);
	}
	return montages;
}

TArray<FString> UCScriptComponent::GetDialDatas(TArray<FScriptData> InDial)
{
	TArray<FString> strs;

	for (int i = 0; i < InDial.Num(); i++)
	{
		strs.Add(InDial[i].Dialogue);
	}

	return strs;
}

FString UCScriptComponent::GetNoneDialData()
{
	if (NoneDialogue.Num() <= DialCount)
	{
		EndDialogue();
		return FString();
	}
	return NoneDialogue[DialCount++].Dialogue;
}

FString UCScriptComponent::GetBeforeDialData()
{
	if (BeforeDialogue.Num() <= DialCount)
	{
		EndDialogue();
		return FString();
	}
	return BeforeDialogue[DialCount++].Dialogue;
}

FString UCScriptComponent::GetAcceptDialData()
{
	if (AcceptDialogue.Num() <= DialCount)
	{
		EndDialogue();
		return FString();
	}
	return AcceptDialogue[DialCount++].Dialogue;
}

FString UCScriptComponent::GetRejectDialData()
{
	if (RejectDialogue.Num() <= DialCount)
	{
		EndDialogue();
		return FString();
	}
	return RejectDialogue[DialCount++].Dialogue;
}

FString UCScriptComponent::GetProceedingDialData()
{
	if (ProceedingDialogue.Num() <= DialCount)
	{
		EndDialogue();
		return FString();
	}
	return ProceedingDialogue[DialCount++].Dialogue;
}

FString UCScriptComponent::GetCompleteDialData()
{
	if (CompleteDialogue.Num() <= DialCount)
	{
		EndDialogue();
		return FString();
	}
	return CompleteDialogue[DialCount++].Dialogue;
}

void UCScriptComponent::AddNoneDialData(const FScriptData * InData)
{
	FScriptData data;

	data = *InData;

	NoneDialogue.Add(data);
}

void UCScriptComponent::AddBeforeDialData(const FScriptData * InData)
{
	FScriptData data;

	data = *InData;

	BeforeDialogue.Add(data);
}

void UCScriptComponent::AddAcceptDialData(const FScriptData * InData)
{
	FScriptData data;

	data = *InData;

	AcceptDialogue.Add(data);
}

void UCScriptComponent::AddRejectDialData(const FScriptData * InData)
{
	FScriptData data;

	data = *InData;

	RejectDialogue.Add(data);
}

void UCScriptComponent::AddProceedingDialData(const FScriptData * InData)
{
	FScriptData data;

	data = *InData;

	ProceedingDialogue.Add(data);
}

void UCScriptComponent::AddCompleteDialData(const FScriptData * InData)
{
	FScriptData data;

	data = *InData;

	CompleteDialogue.Add(data);
}

