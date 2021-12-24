#include "CClass.h"
#include "Global.h"

#include "CSkill.h"
#include "Components/CSkillComponent.h"
#include "Components/CStateComponent.h"

#include "Components/CMontagesComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/ShapeComponent.h"

#include "PaperSpriteComponent.h"

ACClass::ACClass()
{
	PrimaryActorTick.bCanEverTick = true;

	Montages = CreateDefaultSubobject<UCMontagesComponent>("Montages");
}

void ACClass::BeginPlay()
{

	OwnerCharacter = Cast<ACharacter>(GetOwner());

	State = CHelpers::GetComponent<UCStateComponent>(OwnerCharacter);
	Skills = CHelpers::GetComponent<UCSkillComponent>(OwnerCharacter);

	FActorSpawnParameters params;
	params.Owner = GetOwner();

	FTransform transform;
	if (AttackClass != NULL)
	{
		Attack = Cast<ACSkill>(GetWorld()->SpawnActor(AttackClass, &transform, params));
		if (Attack != NULL)
		{
			TArray<UShapeComponent *> components;
			GetShapeComponent(components);
			Attack->SetClassColliders(components);
			//Attack->SetSkillButton(ESkillButtonType::Q);
		}
	}

	if (QSkillClass != NULL)
	{
		QSkill = Cast<ACSkill>(GetWorld()->SpawnActor(QSkillClass, &transform, params));
		if (QSkill != NULL)
		{
			/*TArray<UShapeComponent *> components;
			GetShapeComponent(components);
			QSkill->SetClassColliders(components);*/
			QSkill->SetSkillButton(ESkillButtonType::Q);
		}
	}

	if (WSkillClass != NULL)
	{
		WSkill = Cast<ACSkill>(GetWorld()->SpawnActor(WSkillClass, &transform, params));
		if (WSkill != NULL)
		{
			/*TArray<UShapeComponent *> components;
			GetShapeComponent(components);
			WSkill->SetClassColliders(components);*/
			WSkill->SetSkillButton(ESkillButtonType::W);
		}
	}

	if (ESkillClass != NULL)
	{
		ESkill = Cast<ACSkill>(GetWorld()->SpawnActor(ESkillClass, &transform, params));
		if (ESkill != NULL)
		{
			/*TArray<UShapeComponent *> components;
			GetShapeComponent(components);
			ESkill->SetClassColliders(components);*/
			ESkill->SetSkillButton(ESkillButtonType::E);
		}
	}

	if (RSkillClass != NULL)
	{
		RSkill = Cast<ACSkill>(GetWorld()->SpawnActor(RSkillClass, &transform, params));
		if (RSkill != NULL)
		{
			//TArray<UShapeComponent *> components;
			//GetShapeComponent(components);
			//RSkill->SetClassColliders(components);
			RSkill->SetSkillButton(ESkillButtonType::R);
		}
	}

	if (ASkillClass != NULL)
	{
		ASkill = Cast<ACSkill>(GetWorld()->SpawnActor(ASkillClass, &transform, params));
		if (ASkill != NULL)
		{
			/*TArray<UShapeComponent *> components;
			GetShapeComponent(components);
			ASkill->SetClassColliders(components);*/
			ASkill->SetSkillButton(ESkillButtonType::A);
		}
	}

	if (SSkillClass != NULL)
	{
		SSkill = Cast<ACSkill>(GetWorld()->SpawnActor(SSkillClass, &transform, params));
		if (SSkill != NULL)
		{
			/*TArray<UShapeComponent *> components;
			GetShapeComponent(components);
			SSkill->SetClassColliders(components);*/
			SSkill->SetSkillButton(ESkillButtonType::S);
		}
	}

	if (DSkillClass != NULL)
	{
		DSkill = Cast<ACSkill>(GetWorld()->SpawnActor(DSkillClass, &transform, params));
		if (DSkill != NULL)
		{
			/*TArray<UShapeComponent *> components;
			GetShapeComponent(components);
			DSkill->SetClassColliders(components);*/
			DSkill->SetSkillButton(ESkillButtonType::D);
		}
	}

	if (FSkillClass != NULL)
	{
		FSkill = Cast<ACSkill>(GetWorld()->SpawnActor(FSkillClass, &transform, params));
		if (FSkill != NULL)
		{
			/*TArray<UShapeComponent *> components;
			GetShapeComponent(components);
			FSkill->SetClassColliders(components);*/
			FSkill->SetSkillButton(ESkillButtonType::F);
		}
	}

	if (DashSkillClass != NULL)
	{
		DashSkill = Cast<ACSkill>(GetWorld()->SpawnActor(DashSkillClass, &transform, params));
		DashSkill->SetSkillButton(ESkillButtonType::Dash);
	}

	Super::BeginPlay();

	Montages->MontagesDataReload();
}

void ACClass::ChangeClass()
{
	Skills->SkillReset();
	Skills->SetAttack(Attack);
	Skills->SetQSkill(QSkill);
	Skills->SetWSkill(WSkill);
	Skills->SetESkill(ESkill);
	Skills->SetRSkill(RSkill);
	Skills->SetASkill(ASkill);
	Skills->SetSSkill(SSkill);
	Skills->SetDSkill(DSkill);
	Skills->SetFSkill(FSkill);
	Skills->SetDashSkill(DashSkill);

	Skills->SetSkillsCanUse(true);
	if(Skills->GetSkillsCanUse())
	CLog::Log("ACClass cpp Line 168" );
}

void ACClass::Equip_Implementation()
{

	const FMontageData_Equip* data = Montages->GetEquipData();
	CheckNull(data);
	//State->SetEquippingMode();

	Play(data->AnimMontage, data->PlayRatio, data->StartSection);

	bEquip = true;
}

void ACClass::Begin_Equip_Implementation()
{
}

void ACClass::End_Equip_Implementation()
{
	State->SetIdleMode();
}

void ACClass::Unequip_Implementation()
{
	bEquip = false;
}

void ACClass::AttachToOwnerMesh(FName InSocket)
{
	CHelpers::AttachToComponent(this, OwnerCharacter->GetMesh(), InSocket);
}

void ACClass::AttachComponentToOwnerMesh(USceneComponent * InComponent, FName InSocket)
{
	InComponent->AttachToComponent(OwnerCharacter->GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), InSocket);
}

void ACClass::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACClass::Play(UAnimMontage * InMontage, float InPlayRatio, FName StartSectionName)
{
	ACharacter* character = Cast<ACharacter>(GetOwner());

	CheckNull(character);
	character->PlayAnimMontage(InMontage, InPlayRatio, StartSectionName);
}

void ACClass::GetShapeComponent(TArray<class UShapeComponent*>& OutComponents)
{
	TArray<UActorComponent *> components = GetComponentsByClass(UShapeComponent::StaticClass());

	for (UActorComponent* component : components)
	{
		UShapeComponent* temp = Cast<UShapeComponent>(component);

		if (!!temp)
			OutComponents.Add(temp);
	}
}



