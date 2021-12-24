#include "CQuestComponent.h"
#include "Global.h"
#include "Quests/CQuest.h"

#include "Characters/CPlayer.h"
#include "Characters/CNpc.h"


UCQuestComponent::UCQuestComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	
}

void UCQuestComponent::BeginPlay()
{
	Super::BeginPlay();

	Player = Cast<ACPlayer>(UGameplayStatics::GetPlayerPawn(this, 0));

	Check(ACQuestClass.Num() != 0, "ACQuest!=NULL");

	FActorSpawnParameters params;
	params.Owner = GetOwner();

	FTransform transform;
	for (int i = 0; i < ACQuestClass.Num(); i++)
	{
		ACQuest* quest = Cast<ACQuest>(GetWorld()->SpawnActor(ACQuestClass[i], &transform, params));
		if (quest != NULL)
		{
			Quest.Add(quest);
			quest->SetPlayer(Player);
			if (quest->GetClearState() == EQuestClearState::Q_Proceeding)
				quest->QuestAccept();
			else if (quest->GetClearState() == EQuestClearState::Q_Complete)
				quest->QuestClear();
		}

	}

}


void UCQuestComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	ACNpc* ownerNpc = Cast<ACNpc>(GetOwner());
	CheckNull(ownerNpc);

	if (GetCanClearList().Num() > 0) //클리어가능 있는지
	{
		if (ownerNpc->GetbHeadUIOnOff() == false)
			ownerNpc->SetHeadUIOnOff(true);
		ownerNpc->SetHeadUIQuestClear();
	}
	else if (GetQuestProceedingList().Num() > 0) //진행중인거 있는지
	{
		if (ownerNpc->GetbHeadUIOnOff() == false)
			ownerNpc->SetHeadUIOnOff(true);
		ownerNpc->SetHeadUIQuestProceeding();
	}
	else if (GetQuestBeforeProgressList().Num() > 0) //진행가능한게 있는지
	{
		if (ownerNpc->GetbHeadUIOnOff() == false)
			ownerNpc->SetHeadUIOnOff(true);
		ownerNpc->SetHeadUIQuestReady();
	}
	else //퀘스트 할 거 없을때
	{
		if (ownerNpc->GetbHeadUIOnOff() == true)
			ownerNpc->SetHeadUIOnOff(false);
	}
}



TArray<ACQuest*> UCQuestComponent::GetQuestList()
{
	TArray<ACQuest*> questList;

	for (int i = 0; i < Quest.Num(); i++)
	{
		if (Quest[i]->GetClearState() == EQuestClearState::Q_BeforeProgress || Quest[i]->GetClearState() == EQuestClearState::Q_Proceeding)
			questList.Add(Quest[i]);
	}
	
	return questList;
}

TArray<ACQuest*> UCQuestComponent::GetQuestBeforeProgressList()
{
	TArray<ACQuest*> questList;

	for (int i = 0; i < Quest.Num(); i++)
	{
		if (Quest[i]->GetClearState() == EQuestClearState::Q_BeforeProgress)
			questList.Add(Quest[i]);
	}

	return questList;
}

TArray<ACQuest*> UCQuestComponent::GetQuestProceedingList()
{
	TArray<ACQuest*> questList;

	for (int i = 0; i < Quest.Num(); i++)
	{
		if (Quest[i]->GetClearState() == EQuestClearState::Q_Proceeding)
		{
			questList.Add(Quest[i]);
		}
	}

	return questList;
}

TArray<ACQuest*> UCQuestComponent::GetCanClearList()
{
	TArray<ACQuest*> questList;

	for (int i = 0; i < Quest.Num(); i++)
	{
		if (Quest[i]->GetClearState() == EQuestClearState::Q_Proceeding && Quest[i]->CheckClearCondition())
			questList.Add(Quest[i]);
	}

	return questList;
}

TArray<FString> UCQuestComponent::GetErrorDialogue()
{
	TArray<FString> dial;
	return dial;
}

FString UCQuestComponent::GetNoneDialogue()
{
	FString str;
	if (Quest.Num() != 0)
		str = Quest[0]->GetNoneDialogue();
	return str;
}


TArray<FString> UCQuestComponent::GetNoneDialDatas()
{
	TArray<FString> strDatas;
	if (Quest.Num() != 0)
		strDatas = Quest[0]->GetDialDatas(EQuestClearState::Q_None);
	return strDatas;
}

FMontageData_Npc UCQuestComponent::GetNoneMontage()
{
	return Quest[0]->GetNoneMontage();
}

TArray<FMontageData_Npc> UCQuestComponent::GetNoneMontages()
{
	return Quest[0]->GetNoneMontages();
}
