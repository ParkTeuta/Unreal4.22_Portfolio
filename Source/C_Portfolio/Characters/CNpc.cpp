#include "CNpc.h"
#include "Global.h"
#include "Animation/AnimInstance.h"
#include "Materials/Material.h"
#include "Materials/MaterialInstanceConstant.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Animation/AnimMontage.h"

#include "Quests/CQuest.h"

#include "Components/TextRenderComponent.h"
#include "Components/CRecipientComponent.h"

#include "CPlayer.h"

#include "Components/WidgetComponent.h"
#include "Widgets/CWidget_NpcHeadUI.h"

ACNpc::ACNpc()
{
	PrimaryActorTick.bCanEverTick = true;

	//NameText = CreateDefaultSubobject<UTextRenderComponent>("NameText");
	//NameText->SetupAttachment(GetMesh());

	
	//Character Mesh
	USkeletalMesh* mesh;
	CHelpers::GetAsset<USkeletalMesh>(&mesh, "SkeletalMesh'/Game/Character/Mesh/SK_Mannequin.SK_Mannequin'");
	GetMesh()->SetSkeletalMesh(mesh);

	GetMesh()->RelativeLocation = FVector(0, 0, -90);
	GetMesh()->RelativeRotation = FRotator(0, -90, 0);

	UMaterialInstanceConstant* body;
	CHelpers::GetAsset<UMaterialInstanceConstant>(&body, "MaterialInstanceConstant'/Game/Materials/M_NPC_Body.M_NPC_Body'");
	GetMesh()->SetMaterial(0, body);

	UMaterialInstanceConstant* logo;
	CHelpers::GetAsset<UMaterialInstanceConstant>(&logo, "MaterialInstanceConstant'/Game/Materials/M_NPC_Logo.M_NPC_Logo'");
	GetMesh()->SetMaterial(1, logo);

	//animation
	TSubclassOf<UAnimInstance> animInstance;
	CHelpers::GetClass<UAnimInstance>(&animInstance, "AnimBlueprint'/Game/NPC/ABP_NPC.ABP_NPC_C'");
	GetMesh()->SetAnimInstanceClass(animInstance);


	QuestComp = CreateDefaultSubobject<UCQuestComponent>("Quest");

	NpcName = FText::FromString("NPC");

	HeadUI = CreateDefaultSubobject<UWidgetComponent>("HeadUI");
	HeadUI->SetupAttachment(GetMesh());

	HeadUI->SetWidgetClass(HeadUIClass);
	HeadUI->RelativeLocation = FVector(0, 0, 185);
	HeadUI->SetDrawSize(FVector2D(120, 15));
	HeadUI->SetWidgetSpace(EWidgetSpace::Screen);


}

void ACNpc::BeginPlay()
{
	Super::BeginPlay();

	SetHeadUIOnOff(false);
}


void ACNpc::SetHeadUIOnOff(bool InOnOff)
{
	UCWidget_NpcHeadUI* headUi = Cast<UCWidget_NpcHeadUI>(HeadUI->GetUserWidgetObject());
	CheckNull(headUi);
	headUi->SetQuestOnOff(InOnOff);
	SetbHeadUIOnOff(InOnOff);
}

void ACNpc::SetHeadUIQuestClear()
{
	UCWidget_NpcHeadUI* headUi = Cast<UCWidget_NpcHeadUI>(HeadUI->GetUserWidgetObject());
	CheckNull(headUi);
	headUi->SetQuestClear();
}

void ACNpc::SetHeadUIQuestProceeding()
{
	UCWidget_NpcHeadUI* headUi = Cast<UCWidget_NpcHeadUI>(HeadUI->GetUserWidgetObject());
	CheckNull(headUi);
	headUi->SetQuestProceeding();
}

void ACNpc::SetHeadUIQuestReady()
{
	UCWidget_NpcHeadUI* headUi = Cast<UCWidget_NpcHeadUI>(HeadUI->GetUserWidgetObject());
	CheckNull(headUi);
	headUi->SetQuestReady();
}

void ACNpc::SetHeadUINpcName()
{
	UCWidget_NpcHeadUI* headUi = Cast<UCWidget_NpcHeadUI>(HeadUI->GetUserWidgetObject());
	CheckNull(headUi);
	headUi->SetNPCName(NpcName.ToString());
}

TArray<FString> ACNpc::DialProcessing()
{
	DialState = EDialogueState::D_None;
	//Complete : 퀘스트 진행 중인 거 있음. 완료할 것도 있음.
	if (GetCanClearList().Num() > 0)
	{
		//DialState = EDialgoueState::D_Clear;
		TArray<ACQuest*> quests = GetCanClearList();
		quests[0]->QuestClear();
		Player->QuestRecipient->QuestClear(quests[0]);
		DialState = EDialogueState::D_Wait;
		TempMontage = quests[0]->GetMontageDataToClearState(EQuestClearState::Q_Complete);
		TempMontages = quests[0]->GetMontageDatasToClearState(EQuestClearState::Q_Complete);
		return quests[0]->GetDialDatas(EQuestClearState::Q_Complete);
	}

	//퀘스트 받을 게 있을 때
	if (GetQuestBeforeProgressList().Num() > 0)
	{
		if (GetQuestProceedingList().Num() > 0)
		{
			DialState = EDialogueState::D_Wait;
			TempMontage = GetQuestProceedingList()[0]->GetMontageDataToClearState(EQuestClearState::Q_Proceeding);
			TempMontages = GetQuestProceedingList()[0]->GetMontageDatasToClearState(EQuestClearState::Q_Proceeding);
			return GetQuestProceedingList()[0]->GetDialDatas(EQuestClearState::Q_Proceeding);
		}
		
		DialState = EDialogueState::D_Wait;
		TempMontage = GetNoneMontage();
		TempMontages = GetNoneMontages();
		return GetNoneDialDatas();
	}

	//Proceeding : 진행중인 거 하나라도 있으므. 퀘스트 완료할 것 없음. 퀘스트 받을 거 없음.
	if (GetQuestProceedingList().Num() > 0)
	{
		DialState = EDialogueState::D_Wait;
		TempMontage = GetQuestProceedingList()[0]->GetMontageDataToClearState(EQuestClearState::Q_Proceeding);
		TempMontages = GetQuestProceedingList()[0]->GetMontageDatasToClearState(EQuestClearState::Q_Proceeding);
		return GetQuestProceedingList()[0]->GetDialDatas(EQuestClearState::Q_Proceeding);
	}

	//None : 퀘스트 받을 거 없음, 퀘스트 진행중인 거 없음. 퀘스트 완료할 것도 없음.
	/*if (GetQuestList().Num() == 0)
	{*/
		DialState = EDialogueState::D_End; //대화 그냥 종료임 ㅇㅋ? 넹.
		TempMontage = GetNoneMontage();
		TempMontages = GetNoneMontages();
		return GetNoneDialDatas();
	//}
}

void ACNpc::PlayProcessingMontage()
{
	if (TempMontage.AnimMontage != NULL)
		PlayAnimMontage(TempMontage.AnimMontage, TempMontage.PlayRatio, TempMontage.StartSection);
	TempMontage = FMontageData_Npc();
}

TArray<FString> ACNpc::SelectQuestByNum(int32 InNum)
{
	TArray<ACQuest*> quests = GetQuestList();
	CheckTrueResult(quests.Num() <= InNum, QuestComp->GetErrorDialogue());
	return SelectQuestByActor(quests[InNum]);
}

TArray<FString> ACNpc::SelectQuestByActor(ACQuest * InQuest)
{
	if (InQuest->GetClearState() == EQuestClearState::Q_Proceeding)
		DialState = EDialogueState::D_Wait;
	else if (InQuest->GetClearState() == EQuestClearState::Q_BeforeProgress)
	{
		TempQuest = InQuest;
		DialState = EDialogueState::D_Accept;
	}

	return InQuest->GetDialogue();
}

void ACNpc::PlayMontageByActor(ACQuest * InQuest)
{
	TempMontages = InQuest->GetDialMontages();
	FMontageData_Npc montage = InQuest->GetDialMontage();
	if (montage.AnimMontage != NULL)
		PlayAnimMontage(montage.AnimMontage, montage.PlayRatio, montage.StartSection);
}

TArray<FString> ACNpc::SelectAccept()
{
	TempQuest->QuestAccept();
	DialState = EDialogueState::D_End;
	return TempQuest->GetDialDatas(EQuestClearState::Q_Accept);
}

TArray<FString> ACNpc::SelectReject()
{
	TempQuest->QuestReject();
	DialState = EDialogueState::D_End;
	return TempQuest->GetDialDatas(EQuestClearState::Q_Reject);
}

void ACNpc::PlayAcceptMontage()
{
	TempMontages = TempQuest->GetMontageDatasToClearState(EQuestClearState::Q_Accept);
	FMontageData_Npc montage = TempQuest->GetMontageDataToClearState(EQuestClearState::Q_Accept);
	if (montage.AnimMontage != NULL)
		PlayAnimMontage(montage.AnimMontage, montage.PlayRatio, montage.StartSection);
}

void ACNpc::PlayRejectMontage()
{
	TempMontages = TempQuest->GetMontageDatasToClearState(EQuestClearState::Q_Reject);
	FMontageData_Npc montage = TempQuest->GetMontageDataToClearState(EQuestClearState::Q_Reject);
	if (montage.AnimMontage != NULL)
		PlayAnimMontage(montage.AnimMontage, montage.PlayRatio, montage.StartSection);
}

void ACNpc::PlayMontageByCount(int32 InCount)
{
	CheckFalse(TempMontages.Num() > 0);
	CheckTrue(TempMontages.Num() <= InCount);
	CheckNull(TempMontages[InCount].AnimMontage);

	PlayAnimMontage(TempMontages[InCount].AnimMontage, TempMontages[InCount].PlayRatio, TempMontages[InCount].StartSection);
}

TArray<ACQuest*> ACNpc::GetQuestList()
{
	return QuestComp->GetQuestList();
}

TArray<ACQuest*> ACNpc::GetQuestBeforeProgressList()
{
	return QuestComp->GetQuestBeforeProgressList();
}

TArray<class ACQuest*> ACNpc::GetQuestProceedingList()
{
	return QuestComp->GetQuestProceedingList();
}

TArray<class ACQuest*> ACNpc::GetCanClearList()
{
	return QuestComp->GetCanClearList();
}

FString ACNpc::GetNoneDialogue()
{
	return QuestComp->GetNoneDialogue();
}

TArray<FString> ACNpc::GetNoneDialDatas()
{
	return QuestComp->GetNoneDialDatas();
}

FMontageData_Npc ACNpc::GetNoneMontage()
{
	return QuestComp->GetNoneMontage();
}

TArray<FMontageData_Npc> ACNpc::GetNoneMontages()
{
	return QuestComp->GetNoneMontages();
}

void ACNpc::OnComponentBeginOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	CheckTrue(OtherActor == this);
	
	Player = Cast<ACPlayer>(OtherActor);
	CheckNull(Player);

	Player->GetRecipient()->SetNpc(this);
}

void ACNpc::OnComponentEndOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	CheckTrue(OtherActor == this);

	Player = Cast<ACPlayer>(OtherActor);
	CheckNull(Player);

	CheckTrue(Player->GetRecipient()->GetNpc() != this);
	Player->GetRecipient()->SetNpc(NULL);
}

void ACNpc::EndDialogue()
{
	DialState = EDialogueState::D_End;
}

void ACNpc::ResetDialogue()
{
	//CheckNull(Player);
	if (Player != NULL)
		Player->GetRecipient()->SetIsWidgetOpen(false);
	DialState = EDialogueState::D_None;
	TempQuest = NULL;
	CameraReset();
}

void ACNpc::CameraMoving_Implementation()
{
}

void ACNpc::CameraReset_Implementation()
{
}

void ACNpc::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACNpc::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

ACPlayer * ACNpc::GetPlayer()
{
	return Player;
}

