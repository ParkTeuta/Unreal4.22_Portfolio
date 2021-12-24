#include "CAnimNotifyState_Equip.h"
#include "Global.h"
#include "Characters/CPlayer.h"
//#include "Weapons/CWeapon.h"
#include "Weapons/CClass.h"
//#include "Components/CCombatComponent.h"
#include "Components/CClassComponent.h"

FString UCAnimNotifyState_Equip::GetNotifyName_Implementation() const
{
	return "Equip";
}

void UCAnimNotifyState_Equip::NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);

	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UCClassComponent* component = CHelpers::GetComponent<UCClassComponent>(MeshComp->GetOwner());
	CheckNull(component);

	ACClass* cclass = component->GetCClass();
	CheckNull(cclass);

	cclass->Begin_Equip();
}

void UCAnimNotifyState_Equip::NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation)
{
	Super::NotifyEnd(MeshComp, Animation);

	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UCClassComponent* component = CHelpers::GetComponent<UCClassComponent>(MeshComp->GetOwner());
	CheckNull(component);

	ACClass* cclass = component->GetCClass();
	CheckNull(cclass);

	cclass->End_Equip();
}