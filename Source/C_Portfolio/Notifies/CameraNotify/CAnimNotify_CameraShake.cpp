#include "CAnimNotify_CameraShake.h"
#include "Global.h"
#include "Camera/PlayerCameraManager.h"
#include "Camera/CameraShake.h"
#include "CameraShake/CCameraShake.h"

UCAnimNotify_CameraShake::UCAnimNotify_CameraShake()
{
	CameraShake = UCCameraShake::StaticClass();
}


FString UCAnimNotify_CameraShake::GetNotifyName_Implementation() const
{
	return "CameraShake";
}

void UCAnimNotify_CameraShake::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	CheckNull(MeshComp);
	APlayerController* controller = UGameplayStatics::GetPlayerController(MeshComp->GetWorld(), 0);
	CheckNull(controller);

	APlayerCameraManager* camera = controller->PlayerCameraManager;
	CheckNull(camera);

	camera->PlayCameraShake(CameraShake);
}
