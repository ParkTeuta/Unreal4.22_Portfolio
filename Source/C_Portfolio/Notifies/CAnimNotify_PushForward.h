#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "CAnimNotify_PushForward.generated.h"

UCLASS()
class C_PORTFOLIO_API UCAnimNotify_PushForward : public UAnimNotify
{
	GENERATED_BODY()
	
public:
	FString GetNotifyName_Implementation() const override;
	virtual void Notify(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation) override;
};
