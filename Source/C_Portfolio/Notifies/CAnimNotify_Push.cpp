#include "CAnimNotify_Push.h"
#include "Global.h"
#include "Characters/CPlayer.h"

UCAnimNotify_Push::UCAnimNotify_Push()
{
	IsForward = true;
}

FString UCAnimNotify_Push::GetNotifyName_Implementation() const
{
	return "Push";
}


void UCAnimNotify_Push::Notify(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation)
{
	Super::Notify(MeshComp, Animation);

	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	ACharacter* character = Cast<ACharacter>(MeshComp->GetOwner());
	CheckNull(character);

	FVector Forward = character->GetActorForwardVector();
	if (IsForward == false)
		Forward = Forward  * -1;
	character->LaunchCharacter(Forward * Power, false, false);
}
