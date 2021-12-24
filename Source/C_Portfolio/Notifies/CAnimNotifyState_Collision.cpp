#include "CAnimNotifyState_Collision.h"
#include "Global.h"
#include "Characters/CPlayer.h"
#include "Characters/CEnemy.h"
#include "Weapons/CSkill.h"
//#include "Abilities/CAbility_Melee.h"
#include "Abilities/CAbility.h"
#include "Components/CSkillComponent.h"
#include "Components/CStateComponent.h"

FString UCAnimNotifyState_Collision::GetNotifyName_Implementation() const
{
	return "Collision";
}

void UCAnimNotifyState_Collision::NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);

	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UCSkillComponent* skillComp = CHelpers::GetComponent<UCSkillComponent>(MeshComp->GetOwner());
	CheckNull(skillComp);

	UCStateComponent* state = CHelpers::GetComponent<UCStateComponent>(MeshComp->GetOwner());
	CheckNull(state);

	ACSkill* skill = NULL;
	if (state->IsAttack())
		skill = skillComp->GetAttack();
	else if (state->IsSkill())
		skill = skillComp->GetSkill();

	CheckNull(skill);

	ACAbility* ability = skill->GetAbility();
	CheckNull(ability);

	ability->OnColliders();

}

void UCAnimNotifyState_Collision::NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation)
{
	Super::NotifyEnd(MeshComp, Animation);

	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UCSkillComponent* component = CHelpers::GetComponent<UCSkillComponent>(MeshComp->GetOwner());
	CheckNull(component);

	UCSkillComponent* skillComp = CHelpers::GetComponent<UCSkillComponent>(MeshComp->GetOwner());
	CheckNull(skillComp);

	UCStateComponent* state = CHelpers::GetComponent<UCStateComponent>(MeshComp->GetOwner());
	CheckNull(state);

	ACSkill* skill = NULL;
	if (state->IsAttack())
		skill = skillComp->GetAttack();
	else if (state->IsSkill())
		skill = skillComp->GetSkill();

	CheckNull(skill);

	ACAbility* ability = skill->GetAbility();
	CheckNull(ability);

	ability->OffColliders();

}