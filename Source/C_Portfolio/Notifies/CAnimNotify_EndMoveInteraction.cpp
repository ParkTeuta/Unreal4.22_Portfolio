#include "CAnimNotify_EndMoveInteraction.h"
#include "Global.h"

#include "Characters/CPlayer.h"
#include "Components/CMoveInteractionComponent.h"
#include "Interactions/CMoveInteraction.h"


FString UCAnimNotify_EndMoveInteraction::GetNotifyName_Implementation() const
{
	return "EndInteraction";
}


void UCAnimNotify_EndMoveInteraction::Notify(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation)
{
	Super::Notify(MeshComp, Animation);

	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	ACPlayer* player = Cast<ACPlayer>(MeshComp->GetOwner());
	CheckNull(player);

	UCMoveInteractionComponent* comp = CHelpers::GetComponent<UCMoveInteractionComponent>(player);
	CheckNull(comp);

	comp->GetMoveInteraction()->NotifyAction(player);
}
