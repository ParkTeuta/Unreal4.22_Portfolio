#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CSkillComponent.generated.h"

UENUM(BlueprintType)
enum class ESkillButtonType : uint8
{
	Q, W, E, R, A, S, D, F, Z, Dash, 
};

//=============================================================================
UENUM(BlueprintType)
enum class SkillInputType : uint8
{
	None, Enemy, Position, Holding, Immediately,
};

//=============================================================================

UENUM(BlueprintType)
enum class SkillPreviewType : uint8
{
	None, Circle, Arrow, Range,
};

//=============================================================================

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class C_PORTFOLIO_API UCSkillComponent : public UActorComponent
{
	GENERATED_BODY()

private:

	UPROPERTY(EditDefaultsOnly, Category = "Skill")
		class TSubclassOf<class ACSkill> AttackClass;

	UPROPERTY(EditDefaultsOnly, Category = "Skill")
		class TSubclassOf<class ACSkill> QSkillClass;

	UPROPERTY(EditDefaultsOnly, Category = "Skill")
		class TSubclassOf<class ACSkill> WSkillClass;

	UPROPERTY(EditDefaultsOnly, Category = "Skill")
		class TSubclassOf<class ACSkill> ESkillClass;

	UPROPERTY(EditDefaultsOnly, Category = "Skill")
		class TSubclassOf<class ACSkill> RSkillClass;

	UPROPERTY(EditDefaultsOnly, Category = "Skill")
		class TSubclassOf<class ACSkill> ASkillClass;

	UPROPERTY(EditDefaultsOnly, Category = "Skill")
		class TSubclassOf<class ACSkill> SSkillClass;

	UPROPERTY(EditDefaultsOnly, Category = "Skill")
		class TSubclassOf<class ACSkill> DSkillClass;

	UPROPERTY(EditDefaultsOnly, Category = "Skill")
		class TSubclassOf<class ACSkill> FSkillClass;

	UPROPERTY(EditDefaultsOnly, Category = "Skill")
		class TSubclassOf<class ACSkill> DashSkillClass;

public:
	UFUNCTION(BlueprintPure, BlueprintCallable)
		FORCEINLINE ESkillButtonType GetType() { return SkillButton; }

	

public:
	FORCEINLINE class ACSkill* GetSkill() { return Skill; }
	FORCEINLINE class ACSkill* GetWaitSkill() { return WaitSkill; }
	FORCEINLINE class ACSkill* GetAttack() { return Attack; }

	class ACSkill* GetSkill(ESkillButtonType InButton);

	bool GetSkillIsHolding(ESkillButtonType InButton);
	bool GetSkillIsImmediately(ESkillButtonType InButton);

	FORCEINLINE class ACSkill* GetQSkill() { return QSkill; }
	FORCEINLINE class ACSkill* GetWSkill() { return WSkill; }
	FORCEINLINE class ACSkill* GetESkill() { return ESkill; }
	FORCEINLINE class ACSkill* GetRSkill() { return RSkill; }
	FORCEINLINE class ACSkill* GetASkill() { return ASkill; }
	FORCEINLINE class ACSkill* GetSSkill() { return SSkill; }
	FORCEINLINE class ACSkill* GetDSkill() { return DSkill; }
	FORCEINLINE class ACSkill* GetFSkill() { return FSkill; }
	FORCEINLINE class ACSkill* GetDashSkill() { return DashSkill; }

	FORCEINLINE void SetIsHolding(bool InBoolean) { IsHolding = InBoolean; }

public:	
	UCSkillComponent();

public:
	UFUNCTION(BlueprintCallable)
		void SetSkill(ESkillButtonType InNewButton);

	void AttackBehavior();
	void AttackCancel();
	bool GetAttackbStop();

	void Behavior();

public:
	void SkillReset();
	void SetAttack(class ACSkill* InSkill);
	void SetQSkill(class ACSkill* InSkill);
	void SetWSkill(class ACSkill* InSkill);
	void SetESkill(class ACSkill* InSkill);
	void SetRSkill(class ACSkill* InSkill);
	void SetASkill(class ACSkill* InSkill);
	void SetSSkill(class ACSkill* InSkill);
	void SetDSkill(class ACSkill* InSkill);
	void SetFSkill(class ACSkill* InSkill);
	void SetDashSkill(class ACSkill* InSkill);

protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintPure)
		FORCEINLINE bool GetSkillsCanUse() { return SkillsCanUse; }
	FORCEINLINE void SetSkillsCanUse(bool InBoolean) { SkillsCanUse = InBoolean; }


	FORCEINLINE void SetEnemy(ACharacter* InEnemy) { Enemy = InEnemy; }
	FORCEINLINE ACharacter* GetEnemy() { return Enemy; }

	FORCEINLINE void SetTargetPosition(FVector InPosition) { TargetPosition = InPosition; }
	FORCEINLINE FVector GetTargetPosition() { return TargetPosition; }

	void SkillReleased(ESkillButtonType InButton);

	void Cancel();

private:
	ESkillButtonType SkillButton;

private:
	bool SkillsCanUse = false;

	class ACSkill* Attack;

	class ACSkill* WaitSkill;
	class ACSkill* Skill;

	class ACSkill* QSkill;
	class ACSkill* WSkill;
	class ACSkill* ESkill;
	class ACSkill* RSkill;
	class ACSkill* ASkill;
	class ACSkill* SSkill;
	class ACSkill* DSkill;
	class ACSkill* FSkill;
	class ACSkill* DashSkill;

	ACharacter* Enemy;
	FVector TargetPosition;

	bool IsHolding = false;
};
