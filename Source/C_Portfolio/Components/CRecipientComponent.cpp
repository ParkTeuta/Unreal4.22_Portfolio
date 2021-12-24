#include "CRecipientComponent.h"
#include "Global.h"

#include "Characters/CPlayer.h"
#include "Quests/CQuest.h"
#include "CQuestComponent.h"
#include "Characters/CNpc.h"

#include "Objects/CInteractionObject.h"

UCRecipientComponent::UCRecipientComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	IsWidgetOpen = false;
}


void UCRecipientComponent::BeginPlay()
{
	Super::BeginPlay();

	
}


void UCRecipientComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

TArray<class ACQuest*> UCRecipientComponent::GetQuestList()
{
	return QuestList;
}

//bool UCRecipientComponent::GetIsWidgetOpen()
//{
//	
//}

ACNpc * UCRecipientComponent::GetNpc()
{
	return Npc;
}

ACInteractionObject * UCRecipientComponent::GetInteractionObject()
{
	return QuestObject;
}

void UCRecipientComponent::SetIsWidgetOpen(bool InBoolean)
{
	IsWidgetOpen = InBoolean;
}

void UCRecipientComponent::AddCount(AActor * InActor, int32 InCount)
{
	for (int i = 0; i < QuestList.Num(); i++)
	{
		if (QuestList[i]->AddCount(InActor, InCount))
		{
			RefreshQuestItem(i);
		}
	}
}

//
//void UCRecipientComponent::RefreshWidget_Implementation(int32 InCount)
//{
//}

void UCRecipientComponent::RefreshQuestItem(int32 InCount)
{
	ACPlayer* player = Cast<ACPlayer>(GetOwner());
	CheckNull(player);
			
	player->RefreshQuestItem(InCount);
}

void UCRecipientComponent::SetObject(ACInteractionObject * InObject)
{
	QuestObject = InObject;
}

void UCRecipientComponent::SetNpc(ACNpc * InNpc)
{
	Npc = InNpc;
}

void UCRecipientComponent::AddQuestList(ACQuest * InQuest)
{
	QuestList.Add(InQuest);

	ACPlayer* player = Cast<ACPlayer>(GetOwner());
	CheckNull(player);
	player->RefreshQuestList();
}

void UCRecipientComponent::DeleteQuest(ACQuest * InQuest)
{
	//QuestList.Remove(InQuest);
	QuestClear(InQuest);

	ACPlayer* player = Cast<ACPlayer>(GetOwner());
	CheckNull(player);
	player->RefreshQuestList();
}

void UCRecipientComponent::SetQuestList(TArray<class ACQuest*> InList)
{

}

void UCRecipientComponent::QuestClear(ACQuest * InQuest)
{
	if (QuestList.Find(InQuest) != INDEX_NONE)
	{
		if (InQuest->GetClearState() == EQuestClearState::Q_Complete)
		{
			//QuestClearList.Add(InQuest);
			QuestList.Remove(InQuest);

			ACPlayer* player = Cast<ACPlayer>(GetOwner());
			CheckNull(player);
			player->RefreshQuestList();

			return;
		}
	}
}

void UCRecipientComponent::CancelInteraction()
{
	if (IsInteraction)
	{
		CheckNull(QuestObject);
		QuestObject->Cancel();
	}
}

void UCRecipientComponent::AddActiveObject(ACInteractionObject * InObject)
{
	ActiveObject.Add(InObject);
	InObject->Activate_Object();
}

bool UCRecipientComponent::CheckCanActive(ACInteractionObject * InObject)
{
	if (ActiveObject.Find(InObject) != INDEX_NONE)
		return true;
	return false;
}

void UCRecipientComponent::DeleteActiveObject(ACInteractionObject * InObject)
{
	ActiveObject.Remove(InObject);
	InObject->DeactivateAll();
}

