#include "CAnimNotify_Hitted.h"
#include "Global.h"
#include "Characters/CPlayer.h"
#include "Components/CStateComponent.h"

FString UCAnimNotify_Hitted::GetNotifyName_Implementation() const
{
	return "Hitted";
}

void UCAnimNotify_Hitted::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UCStateComponent* component = CHelpers::GetComponent<UCStateComponent>(MeshComp->GetOwner());
	CheckNull(component);
	component->SetIdleMode();
}
