#include "CAbility_Enemy_HerakliosAttack.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Characters/CPlayer.h"
#include "Characters/CEnemy.h"
#include "Components/CSkillComponent.h"
#include "Components/CStateComponent.h"
#include "Components/ShapeComponent.h"
#include "Animation/AnimMontage.h"
#include "Weapons/CSkill.h"
#include "Weapons/CEnemyThrowItem_Parabola.h"
#include "GameFramework/ProjectileMovementComponent.h"

void ACAbility_Enemy_HerakliosAttack::BeginPlay()
{
	Super::BeginPlay();

	TArray<UShapeComponent*> components;
	OwnerSkill->GetShapeComponent(components);
	InputType = SkillInputType::Enemy;

}

void ACAbility_Enemy_HerakliosAttack::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

void ACAbility_Enemy_HerakliosAttack::Behavior()
{
	Super::Behavior();

	
	ACEnemy* enemy = Cast<ACEnemy>(OwnerCharacter);
	CheckNull(enemy);
	CheckNull(enemy->GetAggro());

	State->SetAttackMode();
	OwnerSkill->SetbReady(false);
	OwnerSkill->SetSkillCool();

	enemy->GetMovementComponent()->StopMovementImmediately();

	FRotator rot = CHelpers::YawRotator(UKismetMathLibrary::FindLookAtRotation(enemy->GetActorLocation(),enemy->GetAggro()->GetActorLocation()));
	enemy->SetActorRotation(rot.Quaternion());


	OwnerCharacter->PlayAnimMontage(MontageData->AnimMontage, MontageData->PlayRatio, MontageData->StartSection);
	HitActors.Empty();
}
void ACAbility_Enemy_HerakliosAttack::Begin_Behavior() //클릭하면
{
	FActorSpawnParameters params;
	params.Owner = this;

	//spawn Item..
	//Item = GetWorld()->SpawnActor<AActor>(ItemClass, FTransform(), params);
	//Item->AttachToActor(OwnerCharacter, FAttachmentTransformRules::KeepRelativeTransform, "hand_r");
}

void ACAbility_Enemy_HerakliosAttack::Middle_Behavior()
{	
	//CheckNull(Item);
	//Item->Destroy();

	//spawn
	FActorSpawnParameters params;
	params.Owner = this;

	ACEnemy* OwnerEnemy = Cast<ACEnemy>(OwnerCharacter);
	CheckNull(OwnerEnemy);
	CheckNull(OwnerEnemy->GetAggro());

	FVector targetPosition = OwnerEnemy->GetAggro()->GetActorLocation();
	FVector startPos = OwnerCharacter->GetMesh()->GetBoneLocation(L"hand_r");
	const FRotator newRot = UKismetMathLibrary::FindLookAtRotation(startPos, targetPosition);

	//spawn throwitem..
	ACEnemyThrowItem_Parabola * throwItem = GetWorld()->SpawnActor<ACEnemyThrowItem_Parabola>(ThrowItem, startPos, FRotator(0, newRot.Yaw, 0), params);
	throwItem->SetOwnerSkill(OwnerSkill);
	//throwItem->SetTargetPosition(targetPosition);
	throwItem->ProjectileComponent->SetVelocityInLocalSpace(FVector(0, 0, 0));

	//  == Fixed Angle Projectile Velocity Calculation == 

	const float Gravity = GetWorld()->GetGravityZ() * -1; // Gravity. (Must be a positive value)
	//const float Gravity = 980.f; // This is the same as the line above (unless your project settings have been changed)
	const float Theta = (40 * PI / 180); // Launch angle in radians (40 being the launch angle in degrees)

	FVector dir = targetPosition - startPos; //direction
	float Sz = dir.Z; // Height difference
	dir.Z = 0; // Remove hight from direction
	float Sx = dir.Size(); // Distance

	const float V = (Sx / cos(Theta)) * FMath::Sqrt((Gravity * 1) / (2 * (Sx * tan(Theta) - Sz)));
	FVector VelocityOutput = FVector(V*cos(Theta), 0, V*sin(Theta));

	throwItem->ProjectileComponent->SetVelocityInLocalSpace(VelocityOutput);

}

void ACAbility_Enemy_HerakliosAttack::End_Behavior()
{
	
	State->SetIdleMode();

}

void ACAbility_Enemy_HerakliosAttack::SkillPreview()
{

}

void ACAbility_Enemy_HerakliosAttack::Cancel()
{
	bReady = false;
	bRangeCheck = false;
}

bool ACAbility_Enemy_HerakliosAttack::ExceptionCharacter(ACharacter * InCharacter)
{
	ACEnemy* enemy = Cast<ACEnemy>(InCharacter);
	CheckNullResult(enemy, false);
	return true;
}
