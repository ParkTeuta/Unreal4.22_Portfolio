#include "CAnimNotify_BeginBehavior.h"
#include "Global.h"
#include "Characters/CPlayer.h"
#include "Weapons/CSkill.h"
#include "Abilities/CAbility.h"
#include "Components/CStateComponent.h"
#include "Components/CSkillComponent.h"

FString UCAnimNotify_BeginBehavior::GetNotifyName_Implementation() const
{
	return "BeginBehavior";
}


void UCAnimNotify_BeginBehavior::Notify(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation)
{
	Super::Notify(MeshComp, Animation);

	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UCSkillComponent* component = CHelpers::GetComponent<UCSkillComponent>(MeshComp->GetOwner());
	CheckNull(component);

	UCStateComponent* state = CHelpers::GetComponent<UCStateComponent>(MeshComp->GetOwner());
	CheckNull(state);
	if (state->IsSkill())
	{
		ACSkill* skill = component->GetSkill();
		CheckNull(skill);

		ACAbility* ability = skill->GetAbility();
		CheckNull(ability);

		ability->Begin_Behavior();

		return;
	}

	if (state->IsAttack())
	{
		ACSkill* attack = component->GetAttack();
		CheckNull(attack);

		ACAbility* ability = attack->GetAbility();
		CheckNull(ability);

		ability->Begin_Behavior();

		return;
	}


}
