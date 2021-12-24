#include "CAnimNotify_EndInteraction.h"
#include "Global.h"

#include "Characters/CPlayer.h"
#include "Components/CRecipientComponent.h"
#include "Objects/CInteractionObject.h"

FString UCAnimNotify_EndInteraction::GetNotifyName_Implementation() const
{
	return "EndInteraction";
}


void UCAnimNotify_EndInteraction::Notify(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation)
{
	Super::Notify(MeshComp, Animation);

	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UCRecipientComponent* component = CHelpers::GetComponent<UCRecipientComponent>(MeshComp->GetOwner());
	CheckNull(component);

	ACInteractionObject* object = component->GetInteractionObject();
	CheckNull(object);

	object->EndInteraction();

}
