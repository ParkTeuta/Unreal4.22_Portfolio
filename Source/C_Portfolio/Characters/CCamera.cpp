#include "CCamera.h"
#include "Global.h"
#include "GameFramework/PlayerController.h"


ACCamera::ACCamera()
{
	PrimaryActorTick.bCanEverTick = true;

	//RootComponent->SetWorldLocation();
}

void ACCamera::BeginPlay()
{
	Super::BeginPlay();
}

void ACCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	CameraMove();
	if ((Left || Right || Top || Bottom) && !Holding)
	{
		FLatentActionInfo latentInfo;
		latentInfo.CallbackTarget = this;
		UKismetSystemLibrary::MoveComponentTo(RootComponent, GetCameraMoveLocation(), RootComponent->GetComponentRotation(), true, false, 0.3f, false, EMoveComponentAction::Move, latentInfo);
	}
	else
	{
		FLatentActionInfo latentInfo;
		latentInfo.CallbackTarget = this;
		UKismetSystemLibrary::MoveComponentTo(RootComponent, GetCameraMoveLocation(), RootComponent->GetComponentRotation(), true, false, 0.3f, false, EMoveComponentAction::Stop, latentInfo);
	}
}

void ACCamera::SetCameraLocation(FVector InLocation)
{
	RootComponent->SetWorldLocation(InLocation);
}

void ACCamera::SetCameraRotation(FRotator InRotator)
{
	RootComponent->SetWorldRotation(InRotator);
}

void ACCamera::SetCameraLocationAndRotation(FVector InLocation, FRotator InRotator)
{
	RootComponent->SetWorldLocationAndRotation(InLocation, InRotator);
}

void ACCamera::CameraMove()
{
	FVector2D viewportSize;
	FVector2D mousePos;
	GetWorld()->GetGameViewport()->GetViewportSize(viewportSize);
	GetWorld()->GetGameViewport()->GetMousePosition(mousePos);
	if (mousePos.X<0 || mousePos.X>viewportSize.X || mousePos.Y<0 || mousePos.Y>viewportSize.Y)
		return;
	Left = (mousePos.X <= (viewportSize.X * SizePercent));
	Right = (mousePos.X >= (viewportSize.X - (viewportSize.X * SizePercent)));
	Top = (mousePos.Y <= (viewportSize.Y * SizePercent));
	Bottom = (mousePos.Y >= (viewportSize.Y - (viewportSize.Y * SizePercent)));
}

FVector ACCamera::GetCameraMoveLocation()
{
	FVector location = FVector(0,0,0);

	if (Left)
		location.Y -= 1;
	else if (Right)
		location.Y += 1;

	if (Top)
		location.X += 1;
	else if (Bottom)
		location.X -= 1;

	return RootComponent->GetComponentLocation() + (location * CameraSpeed);
}
