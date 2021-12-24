#include "CClearConditionComponent.h"
#include "Global.h"
#include "Interface/CQuestInterface.h"
#include "Characters/CEnemy.h"

UCClearConditionComponent::UCClearConditionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	Content = L"Get _Condition_ of _Check_ : _Count_";
}


void UCClearConditionComponent::BeginPlay()
{
	Super::BeginPlay();

	//CLog::Log(CheckActor.Get()->GetName());
	//CLog::Log(CheckActor.Get()->GetDisplayNameText().ToString());
	
	//Cast< CheckActor.GetDefaultObject()>
	IsClear = false;
	CheckSuccess();
}

bool UCClearConditionComponent::CheckSuccess()
{
	if (CheckCount <= Count)
	{
		IsClear = true;
	}
	return IsClear;
}

FString UCClearConditionComponent::GetContent()
{
	
	/*FString name = GetName();
	name = name.Replace(L"BP_CEnemy_", L"");
	NameText->Text = FText::FromString(name);*/
	FString str = Content;
	if (CheckClass != NULL) 
	{
		if (CheckClass->ImplementsInterface(UCQuestInterface::StaticClass()) == true)
		{
			FString questObject = ICQuestInterface::Execute_GetObjectName(CheckClass->GetDefaultObject());
			str = str.Replace(L"_Condition_", *questObject);
		}
	}

	FString needCount = FString::FromInt(CheckCount);
	FString nowCount = FString::FromInt(Count);
	str = str.Replace(L"_Check_", *needCount);
	str = str.Replace(L"_Count_", *nowCount);

	return str;
}

bool UCClearConditionComponent::AddCount(AActor * InActor, int32 InCount)
{
	CheckNullResult(CheckClass, false);
	CheckTrueResult(IsClear, false);
	if (CheckClass == InActor->GetClass())
	{
		Count += InCount;
		CheckSuccess();
		return true;
	}
	else if (InActor->GetClass()->IsChildOf(CheckClass))
	{
		Count += InCount;
		CheckSuccess();
		return true;
	}

	return false;
}


void UCClearConditionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

