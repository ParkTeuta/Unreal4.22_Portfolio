#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CStateComponent.generated.h"


UENUM(BlueprintType)
enum class EStateType : uint8
{
	Idle, Attack, Skill, Dead, Dash, Hitted, None, Equipping, Unequipping, MoveInteraction,
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FStateTypeChanged, EStateType, InPrevType, EStateType, InNewType);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class C_PORTFOLIO_API UCStateComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintPure, BlueprintCallable)
		FORCEINLINE EStateType GetStateType() { return Type; }

	UFUNCTION(BlueprintPure, BlueprintCallable)
		FORCEINLINE bool IsDead() { return Type == EStateType::Dead; }

	UFUNCTION(BlueprintPure, BlueprintCallable)
		FORCEINLINE bool IsAlive() { return Type != EStateType::Dead; }

	UFUNCTION(BlueprintPure, BlueprintCallable)
		FORCEINLINE bool IsIdle() { return Type == EStateType::Idle; }

	UFUNCTION(BlueprintPure, BlueprintCallable)
		FORCEINLINE bool IsAttack() { return Type == EStateType::Attack; }

	UFUNCTION(BlueprintPure, BlueprintCallable)
		FORCEINLINE bool IsDash() { return Type == EStateType::Dash; }

	UFUNCTION(BlueprintPure, BlueprintCallable)
		FORCEINLINE bool IsSkill() { return Type == EStateType::Skill; }

	UFUNCTION(BlueprintPure, BlueprintCallable)
		FORCEINLINE bool IsHitted() { return Type == EStateType::Hitted; }

	UFUNCTION(BlueprintPure, BlueprintCallable)
		FORCEINLINE bool IsNone() { return Type == EStateType::None; }

	UFUNCTION(BlueprintPure, BlueprintCallable)
		FORCEINLINE bool IsMoveInteraction() { return Type == EStateType::MoveInteraction; }

public:
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
		FStateTypeChanged OnStateTypeChanged;

public:	
	UCStateComponent();
	UFUNCTION(BlueprintCallable)
		void SetIdleMode();

	void SetDeadMode();

	void SetEquippingMode();
	void SetUnequippingMode();

	void SetAttackMode();
	void SetSkillMode();

	void SetDashMode();

	void SetHittedMode();

	UFUNCTION(BlueprintCallable)
		void SetNoneMode();

	void SetMoveInteractionMode();

protected:
	virtual void BeginPlay() override;

private:
	void ChangeType(EStateType InNewType);

private:
	EStateType Type;
};
