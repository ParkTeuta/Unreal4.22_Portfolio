#include "CAbility_Enemy.h"
#include "Global.h"
#include "Weapons/CSkill.h"
#include "Characters/CEnemy.h"

ACAbility_Enemy::ACAbility_Enemy()
{
	PrimaryActorTick.bCanEverTick = true;

	PreviewType = SkillPreviewType::None;
}

void ACAbility_Enemy::BeginPlay()
{
	Super::BeginPlay();
}

void ACAbility_Enemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	ACEnemy* enemy = Cast<ACEnemy>(OwnerCharacter);
	if (enemy != NULL)
	{
		if (bReady && bRangeCheck)
		{
			CheckNull(enemy->GetAggro());
			bRangeCheck = false;
			FVector location;
			location = enemy->GetAggro()->GetActorLocation();
			FRotator rot = CHelpers::YawRotator(UKismetMathLibrary::FindLookAtRotation(enemy->GetActorLocation(), location));
			enemy->SetActorRotation(rot.Quaternion());

			Behavior();
		}

	}
}

bool ACAbility_Enemy::ExceptionCharacter(ACharacter * InCharacter)
{
	ACEnemy* enemy = Cast<ACEnemy>(InCharacter);
	CheckNullResult(enemy, false);
	return true;
}

