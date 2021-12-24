#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "CAnimNotify_Push.generated.h"

UCLASS()
class C_PORTFOLIO_API UCAnimNotify_Push : public UAnimNotify
{
	GENERATED_BODY()

public:
	//UPROPERTY(BlueprintReadWrite, EditAnywhere)
	//	FVector Direction;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float Power;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		bool IsForward;
public:
	UCAnimNotify_Push();
	FString GetNotifyName_Implementation() const override;
	virtual void Notify(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation) override;
};
