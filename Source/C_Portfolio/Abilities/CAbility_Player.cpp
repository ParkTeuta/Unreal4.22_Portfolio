#include "CAbility_Player.h"
#include "Global.h"
#include "Weapons/CSkill.h"
#include "Characters/CPlayer.h"

ACAbility_Player::ACAbility_Player()
{
	PrimaryActorTick.bCanEverTick = true;

}

void ACAbility_Player::BeginPlay()
{
	Super::BeginPlay();
}

void ACAbility_Player::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	ACPlayer* player = Cast<ACPlayer>(OwnerCharacter);
	if (player != NULL)
	{
		if (bReady && bRangeCheck)
		{
			FVector location;
			if (InputType == SkillInputType::Enemy)
				location = OwnerSkill->GetSkillComp()->GetEnemy()->GetActorLocation();
			else if (InputType == SkillInputType::Position)
				location = OwnerSkill->GetSkillComp()->GetTargetPosition();

			if (FVector::Distance(player->GetActorLocation(), location) > OwnerSkill->GetRange())
				player->MoveToTarget(location);
			else
			{
				bRangeCheck = false;
				//attackMotion
				player->StopMove();
				FRotator rot = CHelpers::YawRotator(UKismetMathLibrary::FindLookAtRotation(player->GetActorLocation(), location));
				player->SetActorRotation(rot.Quaternion());

				Behavior();
			}
		}
	}

}
