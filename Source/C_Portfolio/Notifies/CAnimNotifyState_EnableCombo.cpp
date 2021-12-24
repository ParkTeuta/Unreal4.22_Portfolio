#include "CAnimNotifyState_EnableCombo.h"
#include "Global.h"
#include "Characters/CPlayer.h"
#include "Components/CStateComponent.h"
#include "Components/CSkillComponent.h"
#include "Weapons/CSkill.h"
#include "Abilities/CAbility.h"
//#include "Weapons/CWeapon.h"
//#include "Abilities/CAbility_Melee.h"
//#include "Components/CCombatComponent.h"

FString UCAnimNotifyState_EnableCombo::GetNotifyName_Implementation() const
{
	return "EnableCombo";
}

void UCAnimNotifyState_EnableCombo::NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);

	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UCSkillComponent* component = CHelpers::GetComponent<UCSkillComponent>(MeshComp->GetOwner());
	CheckNull(component);

	UCStateComponent* state = CHelpers::GetComponent<UCStateComponent>(MeshComp->GetOwner());
	CheckNull(state);
	ACAbility* ability = NULL;
	if (state->IsSkill())
	{
		ACSkill* skill = component->GetSkill();
		CheckNull(skill);

		ability = skill->GetAbility();
		CheckNull(ability);

	}

	if (state->IsAttack())
	{
		ACSkill* attack = component->GetAttack();
		CheckNull(attack);

		ability = attack->GetAbility();
		CheckNull(ability);

	}

	ability->OnNextCombo();
}

void UCAnimNotifyState_EnableCombo::NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation)
{
	Super::NotifyEnd(MeshComp, Animation);

	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UCSkillComponent* component = CHelpers::GetComponent<UCSkillComponent>(MeshComp->GetOwner());
	CheckNull(component);

	UCStateComponent* state = CHelpers::GetComponent<UCStateComponent>(MeshComp->GetOwner());
	CheckNull(state);
	ACAbility* ability = NULL;
	if (state->IsSkill())
	{
		ACSkill* skill = component->GetSkill();
		CheckNull(skill);

		ability = skill->GetAbility();
		CheckNull(ability);

	}

	if (state->IsAttack())
	{
		ACSkill* attack = component->GetAttack();
		CheckNull(attack);

		ability = attack->GetAbility();
		CheckNull(ability);

	}

	ability->OffNextCombo();
}