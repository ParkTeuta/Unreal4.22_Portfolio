#include "CWidget_Dialogue.h"
#include "Global.h"
#include "Characters/CPlayer.h"
#include "Characters/CNpc.h"
#include "Quests/CQuest.h"


#include "Components/CRecipientComponent.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/MultiLineEditableTextBox.h"
#include "Components/ListView.h"

void UCWidget_Dialogue::SetWidgets(UButton * InNextButton, UButton * InCloseButton, UButton * InAcceptButton, UButton * InRejectButton, UTextBlock * InNpcNameBlock, UMultiLineEditableTextBox * InContextBox, UListView * InQuestList)
{
	NextButton = InNextButton;
	NextButton->OnClicked.AddDynamic(this, &UCWidget_Dialogue::OnNextButtonClicked);

	CloseButton = InCloseButton;
	CloseButton->OnClicked.AddDynamic(this, &UCWidget_Dialogue::OnCloseButtonClicked);

	AcceptButton = InAcceptButton;
	AcceptButton->OnClicked.AddDynamic(this, &UCWidget_Dialogue::OnAcceptButtonClicked);

	RejectButton = InRejectButton;
	RejectButton->OnClicked.AddDynamic(this, &UCWidget_Dialogue::OnRejectButtonClicked);

	NpcNameBlock = InNpcNameBlock;
	ContextBox = InContextBox;

	QuestList = InQuestList;
	//QuestList->OnItemClicked.AddDynamic(this, &UCWidget_Dialogue::OnQuestListItemClicekd);
	//QuestList->OnItemClicked.AddDynamic(this, &UCWidget_Dialogue::OnQuestListItemClicekd);
}

void UCWidget_Dialogue::OnNextButtonClicked()
{
	NextScript();
	CheckDialEnd();
}

void UCWidget_Dialogue::OnCloseButtonClicked()
{
	Reset();

}

void UCWidget_Dialogue::OnAcceptButtonClicked()
{
	OffSelectButton();
	StartDialogue(Npc->SelectAccept());
	Npc->PlayAcceptMontage();
	CheckDialEnd();
}

void UCWidget_Dialogue::OnRejectButtonClicked()
{
	OffSelectButton();
	StartDialogue(Npc->SelectReject());
	Npc->PlayRejectMontage();
	CheckDialEnd();
}

void UCWidget_Dialogue::OnQuestListItemClicekd(UObject * Item)
{
	CheckNull(Npc);
	ACQuest* quest = Cast<ACQuest>(Item);
	CheckNull(quest);
	StartDialogue(Npc->SelectQuestByActor(quest));
	Npc->PlayMontageByActor(quest);
	CheckDialEnd();
}

void UCWidget_Dialogue::SetNpc(ACNpc* InNpc)
{
	Npc = InNpc;
	NpcNameBlock->SetText(Npc->NpcName);
	StartDialogue(Npc->DialProcessing());
	Npc->PlayProcessingMontage();
	CheckDialEnd();
}

void UCWidget_Dialogue::NextScript()
{
	if (!(DialDatas.Num() <= Count))
	{
		ContextBox->SetText(FText::FromString(DialDatas[Count]));
		Npc->PlayMontageByCount(Count);
	}
}

void UCWidget_Dialogue::Reset()
{
	CheckNull(Npc);
	Npc->ResetDialogue();
	Npc->CameraReset();
	Npc->StopAnimMontage();
	ACPlayer* player = Npc->GetPlayer();

	IsEnd = false;
	Count = 0;
	DialDatas.Empty();

	NextButtonOn();
	//RefreshListBox();
	OnOffListBox(ESlateVisibility::Collapsed);

	OffSelectButton();

	if (player != NULL)
	{
		player->SetPlayerUIVisibility(ESlateVisibility::Visible);
	}
	SetVisibility(ESlateVisibility::Collapsed);

	//Npc = NULL;
}

void UCWidget_Dialogue::OnSelectButton()
{
	NextButton->SetVisibility(ESlateVisibility::Collapsed);
	CloseButton->SetVisibility(ESlateVisibility::Collapsed);
	AcceptButton->SetVisibility(ESlateVisibility::Visible);
	RejectButton->SetVisibility(ESlateVisibility::Visible);
}

void UCWidget_Dialogue::OnQuestList()
{
	RefreshListBox();
	OnOffListBox(ESlateVisibility::Visible);
}

void UCWidget_Dialogue::EndButtonOn()
{
	NextButton->SetVisibility(ESlateVisibility::Collapsed);
	OffSelectButton();
	CloseButton->SetVisibility(ESlateVisibility::Visible);
}

void UCWidget_Dialogue::NextButtonOn()
{
	CloseButton->SetVisibility(ESlateVisibility::Collapsed);
	NextButton->SetVisibility(ESlateVisibility::Visible);
}

void UCWidget_Dialogue::RefreshListBox()
{
	QuestList->ClearListItems();
	for (ACQuest* quest : Npc->GetQuestList())
	{
		QuestList->AddItem(quest);
	}
}

void UCWidget_Dialogue::OnOffListBox(ESlateVisibility InVisibility)
{
	QuestList->SetVisibility(InVisibility);
}

void UCWidget_Dialogue::StartDialogue(TArray<FString> InDialDatas)
{
	DialDatas = InDialDatas;
	ContextBox->SetText(FText::FromString(DialDatas[0]));
	Count = 0;
	IsDialFinished = false;
	NextButtonOn();
	OnOffListBox(ESlateVisibility::Collapsed);
}

void UCWidget_Dialogue::OffSelectButton()
{
	AcceptButton->SetVisibility(ESlateVisibility::Collapsed);
	RejectButton->SetVisibility(ESlateVisibility::Collapsed);
}

void UCWidget_Dialogue::CheckDialEnd()
{
	CheckNull(Npc);
	Count++;
	if (Count >= DialDatas.Num())
	{
		IsDialFinished = true;
	}
	if (IsDialFinished)
	{
		switch (Npc->GetDialState())
		{
		case EDialogueState::D_None:
		{
			Npc->DialProcessing();
			break;
		}
		case EDialogueState::D_End:
		{
			IsEnd = true;
			EndButtonOn();
			break;
		}
		case EDialogueState::D_Accept:
		{
			OnSelectButton();
			break;
		}
		case EDialogueState::D_Wait:
		{
			OnQuestList();
			EndButtonOn();
			break;
		}
		default:
			break;
		}
	}
}

void UCWidget_Dialogue::IfPressG()
{
	CheckNull(Npc);
	if (IsEnd == true)
		OnCloseButtonClicked();
	else if (IsDialFinished)
	{
		switch (Npc->GetDialState())
		{
		case EDialogueState::D_End:
		{
			Reset();
			break;
		}
		case EDialogueState::D_Accept:
		{
			OnAcceptButtonClicked();
			break;
		}
		case EDialogueState::D_Wait:
		{
			if (Npc->GetQuestBeforeProgressList().Num() >= 1)
			{
				StartDialogue(Npc->SelectQuestByActor(Npc->GetQuestBeforeProgressList()[0]));
				CheckDialEnd();
			}
			else
				OnCloseButtonClicked();
			break;
		}
		default:
			break;
		}
	}
	else
		OnNextButtonClicked();
}
