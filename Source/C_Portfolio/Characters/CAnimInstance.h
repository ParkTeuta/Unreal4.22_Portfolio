#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Components/CStateComponent.h"
#include "Components/CClassComponent.h"
#include "CAnimInstance.generated.h"

UCLASS()
class C_PORTFOLIO_API UCAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float Speed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float Direction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		EClassType ClassType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		EStateType StateType;


public:
	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UFUNCTION()
		void ClassTypeChanged(EClassType InPrevType, EClassType InNewType);
	UFUNCTION()
		void StateTypeChanged(EStateType InPrevType, EStateType InNewType);
};
