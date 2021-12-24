#include "CQuest.h"
#include "Global.h"
#include "Characters/CPlayer.h"
#include "Characters/CNpc.h"

#include "Animation/AnimMontage.h"

#include "Components/CClearConditionComponent.h"
#include "Components/CScriptComponent.h"
#include "Components/CRecipientComponent.h"

#include "Objects/CInteractionObject.h"
#include "Interface/CQuestInterface.h"

ACQuest::ACQuest()
{
	PrimaryActorTick.bCanEverTick = true;

	ClearState = EQuestClearState::Q_None;
	Script = CreateDefaultSubobject<UCScriptComponent>("Script");
	ClearCondition = CreateDefaultSubobject<UCClearConditionComponent>("ClearCondition");
}

void ACQuest::BeginPlay()
{
	Super::BeginPlay();
	

}

void ACQuest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CheckCancelCondition();
	CheckCondition();
}


void ACQuest::CheckCondition()
{
	CheckTrue(ClearState != EQuestClearState::Q_None);
	CheckTrue(bCantProgress);

	if (QuestCondition == NULL)
	{
		ClearState = EQuestClearState::Q_BeforeProgress;
		return;
	}

	
	if (GetQuestConditionState(QuestCondition) == EQuestClearState::Q_Complete)
	{
		ClearState = EQuestClearState::Q_BeforeProgress;
		return;
	}
	//CheckNull(Player);
	/*TArray<ACQuest*> quest = Player->QuestRecipient->GetQuestClearList();
	for (int i = 0; i < quest.Num(); i++)
	{
		if (quest[i]->GetClass() == QuestCondition)
		{
			ClearState = EQuestClearState::Q_BeforeProgress;
			return;
		}
	}*/
	/*if (QuestCondition->GetClearState() == EQuestClearState::Q_Complete)
	{
		ClearState = EQuestClearState::Q_BeforeProgress;
	}*/
}

void ACQuest::CheckCancelCondition()
{
	CheckTrue(bCantProgress);
	if (GetQuestConditionState(QuestCancelCondition) == EQuestClearState::Q_Complete)
	{
		/*TArray<ACQuest*> quest = Player->QuestRecipient->GetQuestClearList();
		for (int i = 0; i < quest.Num(); i++)
		{
			if (QuestCancelCondition != NULL)
			{
				if (quest[i]->GetClass() == QuestCancelCondition)
				{*/
		ClearState = EQuestClearState::Q_None;
		bCantProgress = true;
		return;
		//	}
		//}
	}
}

bool ACQuest::CheckClearCondition()
{
	CheckTrueResult(ClearState != EQuestClearState::Q_Proceeding, false);
	return ClearCondition->GetIsClear();
}

FString ACQuest::GetQuestContent_Implementation()
{
	CLog::Log("GetQuestContent Line 69");
	return ClearCondition->GetContent();
}

TArray<FString> ACQuest::GetDialogue()
{
	return GetDialDatas(ClearState);
}

TArray<FString> ACQuest::GetDialDatas(EQuestClearState InState)
{
	return Script->GetDialogueDatas(InState);
}

FMontageData_Npc ACQuest::GetDialMontage()
{
	return Script->GetDialMontage(ClearState);
}

TArray<FMontageData_Npc> ACQuest::GetDialMontages()
{
	return TArray<FMontageData_Npc>();
}

FMontageData_Npc ACQuest::GetMontageDataToClearState(EQuestClearState InState)
{
	return Script->GetDialMontage(InState);
}

TArray<FMontageData_Npc> ACQuest::GetMontageDatasToClearState(EQuestClearState InState)
{
	return Script->GetDialMontages(InState);
}

bool ACQuest::AddCount(AActor * InActor, int32 InCount)
{
	return ClearCondition->AddCount(InActor, InCount);
}

FString ACQuest::GetNoneDialogue()
{
	return Script->GetDialogue(EQuestClearState::Q_None);
}

FMontageData_Npc ACQuest::GetNoneMontage()
{
	return Script->GetDialMontage(EQuestClearState::Q_None);
}

TArray<FMontageData_Npc> ACQuest::GetNoneMontages()
{
	return Script->GetDialMontages(EQuestClearState::Q_None);
}

bool ACQuest::GetIsConditionMet()
{
	if (ClearCondition->GetIsClear())
	{
		OffInteractionObjects();
	}
	return ClearCondition->GetIsClear(); 
}

void ACQuest::SetClearState(EQuestClearState InState)
{
	ClearState = InState;
}

int32 ACQuest::GetCount()
{
	return ClearCondition->GetCount();
}

void ACQuest::OffInteractionObjects()
{
	if (ClearCondition->CheckClass->IsChildOf(ACInteractionObject::StaticClass()))
	{
		TArray<AActor*> actors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ClearCondition->CheckClass, actors);
		for (int i = 0; i < actors.Num(); i++)
		{
			ACInteractionObject* object = Cast<ACInteractionObject>(actors[i]);
			if (object != NULL)
			{
				object->SetbQuestOn(false);
				Player->GetRecipient()->DeleteActiveObject(object);
			}
		}
	}
	else
	{
		TArray<AActor*> actors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ClearCondition->CheckClass, actors);
		for (int i = 0; i < actors.Num(); i++)
		{
			if (actors[i]->GetClass()->ImplementsInterface(UCQuestInterface::StaticClass()) == true)
			{
				ICQuestInterface::Execute_OffVisibility(actors[i]);
			}
		}
	}
}

void ACQuest::EndDialogue()
{
}

void ACQuest::SelectQuest()
{

}

void ACQuest::QuestAccept()
{
	if (Player != NULL)
	{
		Player->GetRecipient()->AddQuestList(this);
		if (!GetIsConditionMet()) //Load시 퀘스트 클리어 조건 만족 돼 있으면 아래 작업 안함
		{
			if (ClearCondition->CheckClass->IsChildOf(ACInteractionObject::StaticClass()))
			{
				TArray<AActor*> actors;
				UGameplayStatics::GetAllActorsOfClass(GetWorld(), ClearCondition->CheckClass, actors);
				for (int i = 0; i < actors.Num(); i++)
				{
					ACInteractionObject* object = Cast<ACInteractionObject>(actors[i]);
					if (object != NULL)
					{
						object->SetbQuestOn(true);
						Player->GetRecipient()->AddActiveObject(object);
					}
				}
			}
			else
			{
				TArray<AActor*> actors;
				UGameplayStatics::GetAllActorsOfClass(GetWorld(), ClearCondition->CheckClass, actors);
				for (int i = 0; i < actors.Num(); i++)
				{
					if (actors[i]->GetClass()->ImplementsInterface(UCQuestInterface::StaticClass()) == true)
					{
						ICQuestInterface::Execute_OnVisibility(actors[i]);
					}
				}
			}
		}
		//Player->GetRecipient()->AddActiveObject(ClearCondition->CheckClass))
		QuestAcceptExtraActive();
		ClearState = EQuestClearState::Q_Proceeding;
	}
}

void ACQuest::QuestAcceptExtraActive_Implementation()
{
}

void ACQuest::QuestReject()
{
	//딱히 처리할 게 없넹..
}

void ACQuest::QuestClear()
{
	CheckNull(Player);

	//Player->GetRecipient()->DeleteQuest(this);

	ClearState = EQuestClearState::Q_Complete;
	QuestClearExtraActive();
}

void ACQuest::QuestClearExtraActive_Implementation()
{
}

void ACQuest::LoadQuest(EQuestClearState InState, int32 InCount)
{
	ClearState = InState;
	ClearCondition->SetCount(InCount);
}
