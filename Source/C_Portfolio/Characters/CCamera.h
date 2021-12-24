#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraActor.h"
#include "CCamera.generated.h"

UCLASS()
class C_PORTFOLIO_API ACCamera : public ACameraActor
{
	GENERATED_BODY()
	

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

public:
	ACCamera();

public:
	void SetCameraLocation(FVector InLocation);
	void SetCameraRotation(FRotator InRotator);
	void SetCameraLocationAndRotation(FVector InLocation,FRotator InRotator);

	void SetHolding(bool InHolding) { Holding = InHolding; }

private:
	void CameraMove();
	FVector GetCameraMoveLocation();

private:
	float SizePercent = 0.1f;
	float CameraSpeed = 100.0f;

	bool Left;
	bool Right;
	bool Top;
	bool Bottom;
	
	bool Holding = false;
};
