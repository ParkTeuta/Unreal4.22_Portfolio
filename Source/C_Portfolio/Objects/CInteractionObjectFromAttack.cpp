#include "CInteractionObjectFromAttack.h"
#include "Global.h"

#include "Components/CapsuleComponent.h"
#include "Characters/CPlayer.h"
#include "Weapons/CAttackObject.h"
#include "Weapons/CSkill.h"




void ACInteractionObjectFromAttack::SetVisibililty(bool InBoolean)
{
	ObjectMesh->SetVisibility(InBoolean);
}

ACInteractionObjectFromAttack::ACInteractionObjectFromAttack()
{
	Capsule = CreateDefaultSubobject<UCapsuleComponent>("Capsule");
	SetRootComponent(Capsule);

	ObjectMesh->SetupAttachment(Capsule);
	ActiveWidget->SetupAttachment(Capsule);
	
	NeedHitCount = 1;
	HitCount = 0;


}

void ACInteractionObjectFromAttack::OnComponentBeginOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	CheckFalse(IsActive);

	ACAttackObject* attackObject = Cast<ACAttackObject>(OtherActor);
	ACSkill* skill = (attackObject != NULL) ? attackObject->GetOwnerSkill() : Cast<ACSkill>(OtherActor);
	if (skill != NULL)
	{
		ACPlayer* player = Cast<ACPlayer>(skill->GetOwner());
		CheckNull(player);
		Player = player;
		CheckFalse(CheckCanInteraction());
		Interaction();
	}
	else
	{
		ACPlayer* player = Cast<ACPlayer>(OtherActor->GetOwner());
		CheckNull(player);
		Player = player;
		CheckFalse(CheckCanInteraction());
		Interaction();
	}
}

void ACInteractionObjectFromAttack::Interaction()
{
	if (NeedHitCount == ++HitCount)
	{
		EndInteraction();
		SetInteraction();
	}
	ShakeObject();
	//Èçµé¸®°í ÀÌ·±°Íµµ ÀÛ¾÷ÇØÁà¾ßµÇ´Âµ­ Èþ.. ÀÏ´Ü ½ºÅµ
}

void ACInteractionObjectFromAttack::EndInteraction()
{
	SetFadeOut();
	//SetVisibililty(false);
}


void ACInteractionObjectFromAttack::Reset()
{
	SetFadeIn();
	//SetVisibililty(true);
	HitCount = 0;
	Player = NULL;
}
