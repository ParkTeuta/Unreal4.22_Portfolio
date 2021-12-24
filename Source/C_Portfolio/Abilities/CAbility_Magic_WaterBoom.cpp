#include "CAbility_Magic_WaterBoom.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Characters/CPlayer.h"
#include "Components/CSkillComponent.h"
#include "Components/CStateComponent.h"
#include "Components/ShapeComponent.h"
#include "Animation/AnimMontage.h"

#include "Weapons/CSkill.h"
#include "Weapons/CThrow_WaterBoom.h"
#include "GameFramework/ProjectileMovementComponent.h"


void ACAbility_Magic_WaterBoom::BeginPlay()
{
	Super::BeginPlay();

	InputType = SkillInputType::Position;
	PreviewType = SkillPreviewType::Range;
}

void ACAbility_Magic_WaterBoom::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

void ACAbility_Magic_WaterBoom::Behavior()
{
	Super::Behavior();

	CheckFalse(OwnerSkill->GetbReady());

	State->SetSkillMode();

	HitActors.Empty();
	ACPlayer* player = Cast<ACPlayer>(OwnerCharacter);
	player->StopMove();
	FRotator rot = CHelpers::YawRotator(UKismetMathLibrary::FindLookAtRotation(player->GetActorLocation(), OwnerSkill->GetSkillComp()->GetTargetPosition()));
	player->SetActorRotation(rot.Quaternion());

	FActorSpawnParameters params2;
	params2.Owner = OwnerCharacter;
	FTransform transform2;

	if (MagicCircleClass != NULL)
	{
		MagicCircle = Cast<AActor>(GetWorld()->SpawnActor(MagicCircleClass, &transform2, params2));
		MagicCircle->AttachToComponent(OwnerCharacter->GetMesh(),FAttachmentTransformRules::KeepRelativeTransform, L"Hand_R");
	}
	
	OwnerCharacter->PlayAnimMontage(MontageData->AnimMontage, MontageData->PlayRatio, MontageData->StartSection);
}
void ACAbility_Magic_WaterBoom::Begin_Behavior() //클릭하면
{
	if (MagicCircle != NULL)
	{
		MagicCircle->Destroy();
		MagicCircle = NULL;
	}

	OwnerSkill->SetbReady(false);
	OwnerSkill->SetSkillCool();
	//Projectile Parabola
	//https://forums.unrealengine.com/t/calculating-projectile-velocity-for-parabolic-arc/90351/15

	//spawn
	FActorSpawnParameters params;
	params.Owner = this;

	FVector targetPosition = OwnerSkill->GetSkillComp()->GetTargetPosition();
	FVector startPos = OwnerCharacter->GetMesh()->GetBoneLocation(L"Hand_R");
	const FRotator newRot = UKismetMathLibrary::FindLookAtRotation(startPos, targetPosition);

	//spawn throwitem..
	ACThrow_WaterBoom * water = GetWorld()->SpawnActor<ACThrow_WaterBoom>(ThrowItem, startPos, FRotator(0, newRot.Yaw, 0), params);
	water->SetOwnerSkill(OwnerSkill);
	water->SetTargetPosition(targetPosition);
	water->ProjectileComponent->SetVelocityInLocalSpace(FVector(0, 0, 0));

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

	water->ProjectileComponent->SetVelocityInLocalSpace(VelocityOutput);
}

void ACAbility_Magic_WaterBoom::End_Behavior()
{
	Cancel();

}

void ACAbility_Magic_WaterBoom::SkillPreview()
{
}

void ACAbility_Magic_WaterBoom::Cancel()
{
	if (MagicCircle != NULL)
	{
		MagicCircle->Destroy();
		MagicCircle = NULL;
	}


	bReady = false;
	bRangeCheck = false;

	State->SetIdleMode();
}
