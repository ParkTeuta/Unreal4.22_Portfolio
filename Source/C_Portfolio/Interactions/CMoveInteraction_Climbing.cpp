#include "CMoveInteraction_Climbing.h"
#include "Global.h"
#include "Characters/CPlayer.h"
#include "Animation/AnimMontage.h"

#include "Components/SplineComponent.h"

void ACMoveInteraction_Climbing::Start(ACPlayer * InPlayer)
{
	InPlayer->GetCharacterMovement()->StopMovementImmediately();
	StartLoc = Spline->GetLocationAtSplinePoint(StartIndex, ESplineCoordinateSpace::World);
	EndLoc = Spline->GetLocationAtSplinePoint(EndIndex, ESplineCoordinateSpace::World);
	FRotator rot = CHelpers::YawRotator(UKismetMathLibrary::FindLookAtRotation(StartLoc, EndLoc));

	InPlayer->SetActorRotation(rot.Quaternion());
	InPlayer->SetActorLocation(StartLoc);

	CheckAction();
	if (IsClimbing == true)
	{
		InPlayer->PlayAnimMontage(ClimbingMontage, ClimbingPlayRatio, ClimbingStartSection);
		InPlayer->GetCharacterMovement()->GravityScale = 0.0f;

		InPlayer->LaunchCharacter(FVector(0, 0, 1) * 0.001f, false, false);

		IsTop = false;
		IsClimbingStop = false;
	}
	else
	{
		InPlayer->PlayAnimMontage(JumpDownMontage, JumpDownPlayRatio, JumpDownStartSection);
		IsDown = false;
	}

}

void ACMoveInteraction_Climbing::Activity(ACPlayer * InPlayer)
{
	if (IsClimbing == true)
	{
		if (IsClimbingStop == true)
			return;
		FVector power = FVector(0,0,1) * MoveSpeed;
		FVector loc = InPlayer->GetActorLocation() + power;

		InPlayer->SetActorLocation(loc,false);
		//InPlayer->SetActorLocation(loc,false,nullptr,ETeleportType::TeleportPhysics);

		float Dist = FVector::Dist(InPlayer->GetActorLocation(), EndLoc);
		if (Dist <= ActinoChangeDist && IsTop==false)
		{
			InPlayer->StopAnimMontage();
			InPlayer->PlayAnimMontage(ClimbingEndMontage, ClimbingEndPlayRatio, ClimbingEndStartSection);
			IsTop = true;
		}
		if (Dist <= RemainDist)
		{
			IsClimbingStop = true;
		}
	}
	else
	{
		if (InPlayer->GetCharacterMovement()->IsFalling() == false && IsDown==true)
		{
			MoveState = EMoveState::MS_Finish;
		}
	}
}

void ACMoveInteraction_Climbing::End(ACPlayer * InPlayer)
{
	InPlayer->GetCharacterMovement()->GravityScale = 1.0f;
	InPlayer->GetCharacterMovement()->StopMovementImmediately();
	InPlayer->StopAnimMontage();
	MoveState = EMoveState::MS_None;
}

void ACMoveInteraction_Climbing::NotifyAction(ACPlayer * InPlayer)
{
	if (IsClimbing)
	{
		InPlayer->SetActorLocation(EndLoc);
		MoveState = EMoveState::MS_Finish;
	}
	else
	{
		FVector loc = StartLoc;
		loc.Z = EndLoc.Z;
		InPlayer->SetActorLocation(loc);
		IsDown = true;
	}
}

void ACMoveInteraction_Climbing::CheckAction()
{
	float StartZ = StartLoc.Z;
	float EndZ = EndLoc.Z;
	if (StartZ > EndZ)
		IsClimbing = false;
	else
		IsClimbing = true;
}
