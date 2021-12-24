#include "CMontagesComponent.h"
#include "Global.h"
#include "Animation/AnimMontage.h"
#include "Particles/ParticleSystem.h"
#include "GameFramework/Character.h"
#include "PaperSpriteComponent.h"

#include "Sound/SoundBase.h"

UCMontagesComponent::UCMontagesComponent()
{
	//CHelpers::GetAsset<UDataTable>(&DataTable, "DataTable'/Game/DataTables/DT_Montages.DT_Montages'"); //player¿ë
}


void UCMontagesComponent::BeginPlay()
{

	Check(DataTable != NULL, "DataTable!=NULL");

	//CHelpers::GetAsset<UDataTable>(&DataTable, "DataTable'/Game/DataTables/DT_Montages.DT_Montages'");
	DataTable->GetAllRows<FMontageData>("", MontageDatas);

	for (const FMontageData* data : MontageDatas)
	{
		switch (data->MontageType)
		{
			case EMontageType::Attack: SetAttackData(data); break;
			case EMontageType::Skill: SetSkillData(data); break;
			case EMontageType::Dash: SetDashData(data); break;

			case EMontageType::Hitted: SetHittedData(data); break;

			case EMontageType::Dead: SetDeadData(data); break;
			case EMontageType::Equip: SetEquipData(data); break;
			case EMontageType::Unequip: SetUnequipData(data); break;
		}
	}
	Super::BeginPlay();
}


void UCMontagesComponent::MontagesDataReload()
{
	Check(DataTable != NULL, "DataTable!=NULL");
	DataTable->GetAllRows<FMontageData>("", MontageDatas);
	AttackDatas.Empty();
	for (const FMontageData* data : MontageDatas)
	{
		switch (data->MontageType)
		{
		case EMontageType::Attack: SetAttackData(data); break;
		case EMontageType::Skill: SetSkillData(data); break;
		case EMontageType::Dash: SetDashData(data); break;

		case EMontageType::Hitted: SetHittedData(data); break;

		case EMontageType::Dead: SetDeadData(data); break;
		case EMontageType::Equip: SetEquipData(data); break;
		case EMontageType::Unequip: SetUnequipData(data); break;
		}
	}
}


void UCMontagesComponent::SetAttackData(const FMontageData * InData)
{
	AttackData = FMontageData();
	AttackData.MontageType = InData->MontageType;
	AttackData.AnimMontage = InData->AnimMontage;
	AttackData.PlayRatio = InData->PlayRatio;
	AttackData.StartSection = InData->StartSection;
	AttackData.bCanMove = InData->bCanMove;
	AttackData.Power = InData->Power;
	AttackData.HitStop = InData->HitStop;
	AttackData.HitEffect = InData->HitEffect;
	AttackData.CoolTime = InData->CoolTime;
	AttackData.Range = InData->Range;
	AttackData.TargetCount = InData->TargetCount;
	AttackData.Radius = InData->Radius;
	AttackData.Sprite = InData->Sprite;
	AttackData.HitSound = InData->HitSound;

	AddAttackData(InData);
}

void UCMontagesComponent::SetSkillData(const FMontageData * InData)
{
	SkillData = FMontageData();
	SkillData.MontageType = InData->MontageType;
	SkillData.AnimMontage = InData->AnimMontage;
	SkillData.PlayRatio = InData->PlayRatio;
	SkillData.StartSection = InData->StartSection;
	SkillData.bCanMove = InData->bCanMove;
	SkillData.Power = InData->Power;
	SkillData.HitStop = InData->HitStop;
	SkillData.HitEffect = InData->HitEffect;
	SkillData.CoolTime = InData->CoolTime;
	SkillData.Range = InData->Range;
	SkillData.TargetCount = InData->TargetCount;
	SkillData.Radius = InData->Radius;
	SkillData.Sprite = InData->Sprite;
	SkillData.HitSound = InData->HitSound;

	AddAttackData(InData);
}

void UCMontagesComponent::SetDashData(const FMontageData * InData)
{
	DashData = FMontageData_Character();
	DashData.AnimMontage = InData->AnimMontage;
	DashData.PlayRatio = InData->PlayRatio;
	DashData.StartSection = InData->StartSection;
	DashData.bCanMove = InData->bCanMove;
}

void UCMontagesComponent::SetHittedData(const FMontageData * InData)
{
	HittedData = FMontageData_Character();
	HittedData.AnimMontage = InData->AnimMontage;
	HittedData.PlayRatio = InData->PlayRatio;
	HittedData.StartSection = InData->StartSection;
	HittedData.bCanMove = InData->bCanMove;
}

void UCMontagesComponent::SetDeadData(const FMontageData * InData)
{
	DeadData = FMontageData_Character();
	DeadData.AnimMontage = InData->AnimMontage;
	DeadData.PlayRatio = InData->PlayRatio;
	DeadData.StartSection = InData->StartSection;
	DeadData.bCanMove = InData->bCanMove;
}

void UCMontagesComponent::SetEquipData(const FMontageData * InData)
{
	EquipData = FMontageData_Equip();
	EquipData.AnimMontage = InData->AnimMontage;
	EquipData.PlayRatio = InData->PlayRatio;
	EquipData.StartSection = InData->StartSection;
	EquipData.bCanMove = InData->bCanMove;
}

void UCMontagesComponent::SetUnequipData(const FMontageData * InData)
{
	UnequipData = FMontageData_Equip();
	UnequipData.AnimMontage = InData->AnimMontage;
	UnequipData.PlayRatio = InData->PlayRatio;
	UnequipData.StartSection = InData->StartSection;
	UnequipData.bCanMove = InData->bCanMove;
}

void UCMontagesComponent::AddAttackData(const FMontageData * InData)
{
	FMontageData data;

	data = *InData;

	AttackDatas.Add(data);
}



const FMontageData_Character * UCMontagesComponent::GetHittedData()
{
	return &HittedData;
}

const FMontageData_Character * UCMontagesComponent::GetDeadData()
{
	return &DeadData;
}

const FMontageData_Equip * UCMontagesComponent::GetEquipData()
{
	return &EquipData;
}

const FMontageData_Equip * UCMontagesComponent::GetUnequipData()
{
	return &UnequipData;
}

const FMontageData * UCMontagesComponent::GetAttackData(int32 InIndex)
{
	return &AttackDatas[InIndex];
}

TArray<FMontageData>* UCMontagesComponent::GetAttackDatas()
{
	return &AttackDatas;
}

const FMontageData * UCMontagesComponent::GetAttackData()
{
	return &AttackData;
}

const FMontageData * UCMontagesComponent::GetSkillData()
{
	return &SkillData;
}
const FMontageData_Character * UCMontagesComponent::GetDashData()
{
	return &DashData;
}
