#include "CSkillComponent.h"
#include "Global.h"
#include "Weapons/CSkill.h"
#include "Abilities/CAbility.h"


UCSkillComponent::UCSkillComponent()
{

}

void UCSkillComponent::BeginPlay()
{
	Super::BeginPlay();

	FActorSpawnParameters params;
	params.Owner = GetOwner();

	FTransform transform;
	if (AttackClass != NULL)
	{
		Attack = Cast<ACSkill>(GetWorld()->SpawnActor(AttackClass, &transform, params));
	}
	if (QSkillClass != NULL)
	{
		QSkill = Cast<ACSkill>(GetWorld()->SpawnActor(QSkillClass, &transform, params));
	}
	if (WSkillClass != NULL)
	{
		WSkill = Cast<ACSkill>(GetWorld()->SpawnActor(WSkillClass, &transform, params));
	}
	if (ESkillClass != NULL)
	{
		ESkill = Cast<ACSkill>(GetWorld()->SpawnActor(ESkillClass, &transform, params));
	}
	if (RSkillClass != NULL)
	{
		RSkill = Cast<ACSkill>(GetWorld()->SpawnActor(RSkillClass, &transform, params));
	}
	if (ASkillClass != NULL)
	{
		ASkill = Cast<ACSkill>(GetWorld()->SpawnActor(ASkillClass, &transform, params));
	}
	if (SSkillClass != NULL)
	{
		SSkill = Cast<ACSkill>(GetWorld()->SpawnActor(SSkillClass, &transform, params));
	}
	if (DSkillClass != NULL)
	{
		DSkill = Cast<ACSkill>(GetWorld()->SpawnActor(DSkillClass, &transform, params));
	}
	if (FSkillClass != NULL)
	{
		FSkill = Cast<ACSkill>(GetWorld()->SpawnActor(FSkillClass, &transform, params));
	}

	if (DashSkillClass != NULL)
	{
		DashSkill = Cast<ACSkill>(GetWorld()->SpawnActor(DashSkillClass, &transform, params));
	}

	SkillsCanUse = true;

}

ACSkill * UCSkillComponent::GetSkill(ESkillButtonType InButton)
{
	switch (InButton)
	{
	case ESkillButtonType::Q:
	{
		return QSkill;
	}
	break;
	case ESkillButtonType::W:
	{
		return WSkill;
	}
	break;
	case ESkillButtonType::E:
	{
		return ESkill;
	}
	break;
	case ESkillButtonType::R:
	{
		return RSkill;
	}
	break;
	case ESkillButtonType::A:
	{
		return ASkill;
	}
	break;
	case ESkillButtonType::S:
	{
		return SSkill;
	}
	break;
	case ESkillButtonType::D:
	{
		return DSkill;
	}
	break;
	case ESkillButtonType::F:
	{
		return FSkill;
	}
	break;
	case ESkillButtonType::Dash:
	{
		return DashSkill;
	}
	break;
	}
	return NULL;
}

bool UCSkillComponent::GetSkillIsHolding(ESkillButtonType InButton)
{
	return GetSkill(InButton)->GetAbility()->GetSkillInputType() == SkillInputType::Holding;
}

bool UCSkillComponent::GetSkillIsImmediately(ESkillButtonType InButton)
{
	return GetSkill(InButton)->GetAbility()->GetSkillInputType() == SkillInputType::Immediately;
}

void UCSkillComponent::SkillReleased(ESkillButtonType InButton)
{
	CheckFalse(IsHolding);
	CheckNull(Skill);
	CheckFalse(Skill->GetbReady());
	if (Skill == GetSkill(InButton))
	{
		Skill->GetAbility()->End_Behavior();
		IsHolding = false;
	}
}

void UCSkillComponent::Cancel()
{
	if (Skill != NULL)
		Skill->Cancel();
	Skill = NULL;
}

void UCSkillComponent::SetSkill(ESkillButtonType InNewButton)
{
	ACSkill* tempSkill = NULL;
	if (WaitSkill != NULL)
	{
		tempSkill = WaitSkill;
		WaitSkill->SetbSkillRange(false);
		WaitSkill = NULL;
	}

	WaitSkill = GetSkill(InNewButton);

	if (WaitSkill != NULL)
		WaitSkill->SetbSkillRange(true);

	if (tempSkill == WaitSkill)
		Behavior();
}

void UCSkillComponent::AttackBehavior()
{
	if (Attack != NULL)
		Attack->Behavior();
}

void UCSkillComponent::AttackCancel()
{
	if (Attack != NULL)
		Attack->Cancel();
}

bool UCSkillComponent::GetAttackbStop()
{
	CheckNullResult(Attack,false);
	return Attack->GetbStop();
}

void UCSkillComponent::Behavior()
{
	if (WaitSkill != NULL)
	{
		Skill = WaitSkill;
		WaitSkill->SetbSkillRange(false);
		if (Skill->GetAbility()->GetSkillInputType() == SkillInputType::Holding)
			IsHolding = true;
		Skill->Behavior();
		WaitSkill = NULL;
	}
}

void UCSkillComponent::SkillReset()
{
	SkillsCanUse = false;
	QSkill = NULL;
	WSkill = NULL;
	ESkill = NULL;
	RSkill = NULL;
	ASkill = NULL;
	SSkill = NULL;
	DSkill = NULL;
	FSkill = NULL;
	DashSkill = NULL;
}

void UCSkillComponent::SetAttack(ACSkill * InSkill)
{
	Attack = InSkill;
}

void UCSkillComponent::SetQSkill(ACSkill * InSkill)
{
	QSkill = InSkill;
}

void UCSkillComponent::SetWSkill(ACSkill * InSkill)
{
	WSkill = InSkill;
}

void UCSkillComponent::SetESkill(ACSkill * InSkill)
{
	ESkill = InSkill;
}

void UCSkillComponent::SetRSkill(ACSkill * InSkill)
{
	RSkill = InSkill;
}

void UCSkillComponent::SetASkill(ACSkill * InSkill)
{
	ASkill = InSkill;
}

void UCSkillComponent::SetSSkill(ACSkill * InSkill)
{
	SSkill = InSkill;
}

void UCSkillComponent::SetDSkill(ACSkill * InSkill)
{
	DSkill = InSkill;
}

void UCSkillComponent::SetFSkill(ACSkill * InSkill)
{
	FSkill = InSkill;
}

void UCSkillComponent::SetDashSkill(ACSkill * InSkill)
{
	DashSkill = InSkill;
}
