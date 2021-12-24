#include "CMoveInteraction_Tightrope.h"
#include "Global.h"
#include "Characters/CPlayer.h"
#include "Animation/AnimMontage.h"

#include "Components/SplineComponent.h"
#include "Components/CStatusComponent.h"

#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "GameFramework/PlayerController.h"

void ACMoveInteraction_Tightrope::Start(ACPlayer * InPlayer)
{
	InPlayer->GetCharacterMovement()->StopMovementImmediately();
	StartLoc = Spline->GetLocationAtSplinePoint(StartIndex, ESplineCoordinateSpace::World);
	EndLoc = Spline->GetLocationAtSplinePoint(EndIndex, ESplineCoordinateSpace::World);
	FRotator rot = CHelpers::YawRotator(UKismetMathLibrary::FindLookAtRotation(StartLoc, EndLoc));

	InPlayer->SetActorRotation(rot.Quaternion());
	InPlayer->SetActorLocation(StartLoc);


	InPlayer->PlayAnimMontage(TightropeMontage, PlayRatio, StartSection);

	/*APlayerController* controller = Cast<APlayerController>(InPlayer->GetController());
	CheckNull(controller);

	InPlayer->GetCharacterMovement()->MaxWalkSpeed = MoveSpeed;

	UAIBlueprintHelperLibrary::SimpleMoveToLocation(controller, EndLoc);*/

	MoveState = EMoveState::MS_Processing;
}

void ACMoveInteraction_Tightrope::Activity(ACPlayer * InPlayer)
{
	CheckFalse(MoveState == EMoveState::MS_Processing);

	/*FRotator rot = CHelpers::YawRotator(UKismetMathLibrary::FindLookAtRotation(StartLoc, EndLoc));

	InPlayer->LaunchCharacter(rot.Vector() * MoveSpeed, false, false);*/
	FRotator rot = CHelpers::YawRotator(UKismetMathLibrary::FindLookAtRotation(StartLoc, EndLoc));
	FVector loc = InPlayer->GetActorLocation() + (rot.Vector() * MoveSpeed);
	InPlayer->SetActorLocation(loc);

	float Dist = FVector::Dist(InPlayer->GetActorLocation(), EndLoc);
	CLog::Log("ACMoveInteraction_Tightrope Dist : " + FString::SanitizeFloat(Dist));

	if (Dist <= RemainDist)
	{
		MoveState = EMoveState::MS_Finish;
	}
}

void ACMoveInteraction_Tightrope::End(ACPlayer * InPlayer)
{
	//UCStatusComponent* comp = CHelpers::GetComponent<UCStatusComponent>(InPlayer);

	//InPlayer->GetCharacterMovement()->MaxWalkSpeed = comp->GetSprintSpeed();
	InPlayer->GetCharacterMovement()->StopMovementImmediately();
	InPlayer->StopAnimMontage();
	MoveState = EMoveState::MS_None;
}
