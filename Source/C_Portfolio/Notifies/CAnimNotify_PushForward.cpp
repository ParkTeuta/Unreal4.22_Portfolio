#include "CAnimNotify_PushForward.h"
#include "Global.h"
#include "Characters/CPlayer.h"
#include "Weapons/CSkill.h"
#include "Abilities/CAbility.h"
#include "Components/CStateComponent.h"
#include "Components/CSkillComponent.h"

FString UCAnimNotify_PushForward::GetNotifyName_Implementation() const
{
	return "PushForward";
}


void UCAnimNotify_PushForward::Notify(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation)
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

		ability->PushForward();
		return;
	}

	if (state->IsAttack())
	{
		ACSkill* attack = component->GetAttack();
		CheckNull(attack);

		ACAbility* ability = attack->GetAbility();
		CheckNull(ability);

		ability->PushForward();
		return;
	}

}
