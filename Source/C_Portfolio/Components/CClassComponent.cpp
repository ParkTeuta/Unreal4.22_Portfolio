#include "CClassComponent.h"
#include "Global.h"
#include "Weapons/CClass.h"

#include "PaperSpriteComponent.h"

UCClassComponent::UCClassComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}


void UCClassComponent::BeginPlay()
{
	Super::BeginPlay();

	FActorSpawnParameters params;
	params.Owner = GetOwner();

	FTransform transform;

	if(MagicClass!=NULL)
		Magic = Cast<ACClass>(GetWorld()->SpawnActor(MagicClass, &transform, params));
	if (FistClass != NULL)
		Fist = Cast<ACClass>(GetWorld()->SpawnActor(FistClass, &transform, params));
	if (SwordClass != NULL)
		Sword = Cast<ACClass>(GetWorld()->SpawnActor(SwordClass, &transform, params));

	if (TutorialClass != NULL)
		Tutorial = Cast<ACClass>(GetWorld()->SpawnActor(TutorialClass, &transform, params));
}


void UCClassComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UCClassComponent::SetMode(EClassType InNewType)
{
	switch (InNewType)
	{
	case EClassType::Magic:
		SetMagicMode();
		break;
	case EClassType::Fist:
		SetFistMode();
		break;
	case EClassType::Sword:
		SetSwordMode();
		break;
	case EClassType::Tutorial:
		SetTutorialMode();
		break;
	default:
		break;
	}
}

void UCClassComponent::SetUnarmedMode()
{
	if (CClass != NULL)
	{
		CClass->Unequip();
		CClass = NULL;
	}
	ChangeType(EClassType::Unarmed);
}

void UCClassComponent::SetMagicMode()
{
	CheckNull(Magic);
	if (CClass != NULL)
	{
		CClass->Unequip();
		CClass = NULL;
	}
	CClass = Magic;
	Magic->Equip();
	ChangeType(EClassType::Magic);
	CClass->ChangeClass();
}

void UCClassComponent::SetFistMode()
{
	CheckNull(Fist);
	if (CClass != NULL)
	{
		CClass->Unequip();
		CClass = NULL;
	}
	CClass = Fist;
	Fist->Equip();
	ChangeType(EClassType::Fist);
	CClass->ChangeClass();
}

void UCClassComponent::SetSwordMode()
{
	CheckNull(Sword);
	if (CClass != NULL)
	{
		CClass->Unequip();
		CClass = NULL;
	}
	CClass = Sword;
	Sword->Equip();
	ChangeType(EClassType::Sword);
	CClass->ChangeClass();
}

void UCClassComponent::SetTutorialMode()
{
	CheckNull(Tutorial);
	if (CClass != NULL)
	{
		CClass->Unequip();
		CClass = NULL;
	}
	CClass = Tutorial;
	Tutorial->Equip();
	ChangeType(EClassType::Tutorial);
	CClass->ChangeClass();
}

void UCClassComponent::ChangeType(EClassType InNewType)
{
	EClassType prevType = Type;
	Type = InNewType;

	if (OnClassTypeChanged.IsBound())
		OnClassTypeChanged.Broadcast(prevType, Type);
}

UPaperSprite * UCClassComponent::GetCClassSprite()
{
	CheckNullResult(CClass,NULL);
	return CClass->GetClassSprite();
}
