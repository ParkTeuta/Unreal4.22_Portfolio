#include "CMoveInteraction_Jump.h"
#include "Global.h"
#include "Characters/CPlayer.h"
#include "Animation/AnimMontage.h"

#include "Components/SplineComponent.h"

void ACMoveInteraction_Jump::Start(ACPlayer * InPlayer)
{
	InPlayer->GetCharacterMovement()->StopMovementImmediately();
	StartLoc = Spline->GetLocationAtSplinePoint(StartIndex, ESplineCoordinateSpace::World);
	EndLoc = Spline->GetLocationAtSplinePoint(EndIndex, ESplineCoordinateSpace::World);

	FRotator rot = CHelpers::YawRotator(UKismetMathLibrary::FindLookAtRotation(StartLoc, EndLoc));

	InPlayer->SetActorRotation(rot.Quaternion());
	InPlayer->SetActorLocation(StartLoc);

	CheckAction();
	if (IsLong==true)
		InPlayer->PlayAnimMontage(LongJumpMontage, LongPlayRatio, LongStartSection);
	else
		InPlayer->PlayAnimMontage(ShortJumpMontage, ShortPlayRatio, ShortStartSection);

	MoveState = EMoveState::MS_Processing;


	//const FRotator newRot = CHelpers::YawRotator(UKismetMathLibrary::FindLookAtRotation(StartLoc, EndLoc));

//  == Fixed Angle Projectile Velocity Calculation == 

	const float Gravity = GetWorld()->GetGravityZ() * -1; // Gravity. (Must be a positive value)
	//const float Gravity = 980.f; // This is the same as the line above (unless your project settings have been changed)
	const float Theta = (40 * PI / 180); // Launch angle in radians (40 being the launch angle in degrees)

	FVector dir = EndLoc - StartLoc; //direction
	float Sz = dir.Z; // Height difference
	dir.Z = 0; // Remove hight from direction
	float Sx = dir.Size(); // Distance

	const float V = (Sx / cos(Theta)) * FMath::Sqrt((Gravity * 1) / (2 * (Sx * tan(Theta) - Sz)));
	FVector VelocityOutput = FVector(V*cos(Theta), 0, V*sin(Theta));

	InPlayer->LaunchCharacter(VelocityOutput * JumpPower, false, false);
}

void ACMoveInteraction_Jump::Activity(ACPlayer * InPlayer)
{
	if (InPlayer->GetCharacterMovement()->IsFalling() == false)
	{
		/*if(IsLong==true)
			InPlayer->PlayAnimMontage(LongJumpMontage, LongPlayRatio, "End");
		else
			InPlayer->PlayAnimMontage(ShortJumpMontage, ShortPlayRatio, "End");
*/
		MoveState = EMoveState::MS_Finish;
	}

}

void ACMoveInteraction_Jump::End(ACPlayer * InPlayer)
{
	InPlayer->GetCharacterMovement()->StopMovementImmediately();
	InPlayer->StopAnimMontage();
	MoveState = EMoveState::MS_None;
}

void ACMoveInteraction_Jump::NotifyAction(ACPlayer * InPlayer)
{
	MoveState = EMoveState::MS_Finish;

}

void ACMoveInteraction_Jump::CheckAction()
{
	float dist = FVector::Dist(StartLoc, EndLoc);
	if (dist >= JumpRange)
		IsLong = true;
	else
		IsLong = false;
}
