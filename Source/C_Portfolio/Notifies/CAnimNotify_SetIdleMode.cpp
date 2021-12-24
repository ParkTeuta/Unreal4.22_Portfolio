#include "CAnimNotify_SetIdleMode.h"
#include "Global.h"
#include "Components/CStateComponent.h"

FString UCAnimNotify_SetIdleMode::GetNotifyName_Implementation() const
{
	return "SetIdle";
}


void UCAnimNotify_SetIdleMode::Notify(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation)
{
	Super::Notify(MeshComp, Animation);

	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UCStateComponent* state = CHelpers::GetComponent<UCStateComponent>(MeshComp->GetOwner());
	CheckNull(state);
	
	CheckNull(MeshComp->GetOwner());

	state->SetIdleMode();

}
