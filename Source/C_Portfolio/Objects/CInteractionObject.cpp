#include "CInteractionObject.h"
#include "Global.h"
#include "Characters/CPlayer.h"

#include "Components/WidgetComponent.h"
#include "Widgets/UCWidget_Object.h"

ACInteractionObject::ACInteractionObject()
{
	PrimaryActorTick.bCanEverTick = true;

	ObjectName = "Object";
	IsActive = true;
	ItemCount = 1;


	ObjectMesh = CreateDefaultSubobject<UStaticMeshComponent>("ObjectMesh");
	SetRootComponent(ObjectMesh);

	ActiveWidget = CreateDefaultSubobject<UWidgetComponent>("ActiveWidget");
	ActiveWidget->SetupAttachment(ObjectMesh);

	TSubclassOf<UUserWidget> widget;
	CHelpers::GetClass<UUserWidget>(&widget, "WidgetBlueprint'/Game/Widgets/BP_CObjectAcvie.BP_CObjectAcvie_C'");
	ActiveWidget->SetWidgetClass(widget);
	ActiveWidget->RelativeLocation = FVector(0, 0, 210);
	ActiveWidget->SetDrawSize(FVector2D(120, 15));
	ActiveWidget->SetWidgetSpace(EWidgetSpace::Screen);

}

void ACInteractionObject::BeginPlay()
{
	Super::BeginPlay();
	
	DeactivateAll();
}

void ACInteractionObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACInteractionObject::OnComponentBeginOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	CheckTrue(OtherActor == this);

	Player = Cast<ACPlayer>(OtherActor);
	CheckNull(Player);

	Player->GetRecipient()->SetObject(this);
}

void ACInteractionObject::OnComponentEndOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	CheckTrue(OtherActor == this);

	Player = Cast<ACPlayer>(OtherActor);
	CheckNull(Player);

	CheckTrue(Player->GetRecipient()->GetInteractionObject() != this);
	Player->GetRecipient()->SetObject(NULL);
}

FString ACInteractionObject::GetObjectName_Implementation()
{
	return ObjectName;
}


void ACInteractionObject::SetInteraction()
{
	AddQuestItem(ItemCount);
	Deactivate_Object();
}

void ACInteractionObject::AddQuestItem(int32 InCount)
{
	Player->AddQuestItem(this, InCount);
}

bool ACInteractionObject::CheckCanInteraction()
{
	CheckNullResult(Player, false);
	return Player->GetRecipient()->CheckCanActive(this);
}

void ACInteractionObject::Activate_Object()
{
	GetWorldTimerManager().ClearTimer(timer);
	Reset();
	IsActive = true;

	if (bQuestOn == true)
	{
		UUCWidget_Object* widget = Cast<UUCWidget_Object>(ActiveWidget->GetUserWidgetObject());
		widget->OnOffActive(true);
	}
}

void ACInteractionObject::Deactivate_Object()
{
	DeactivateAll();

	GetWorldTimerManager().SetTimer(timer, this, &ACInteractionObject::Activate_Object, ActivationTime);
	
}

void ACInteractionObject::DeactivateAll()
{
	IsActive = false;
	UUCWidget_Object* widget = Cast<UUCWidget_Object>(ActiveWidget->GetUserWidgetObject());
	widget->OnOffActive(false);
}

