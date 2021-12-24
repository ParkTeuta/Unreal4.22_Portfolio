#include "CAnimNotify_Dead.h"
#include "Global.h"
#include "Characters/CPlayer.h"

FString UCAnimNotify_Dead::GetNotifyName_Implementation() const
{
	return "Dead";
}

void UCAnimNotify_Dead::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UCStateComponent* component = CHelpers::GetComponent<UCStateComponent>(MeshComp->GetOwner());
	CheckNull(component);
	component->SetNoneMode();
}
