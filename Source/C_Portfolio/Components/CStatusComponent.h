#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CStatusComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class C_PORTFOLIO_API UCStatusComponent : public UActorComponent
{
	GENERATED_BODY()
		
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		float MaxHealth = 600;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		float MaxMp = 100;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly) //ad
		float AttackDamage = 65;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly) //ap
		float AbilityPower = 0;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly) //as
		float AttackSpeed = 0.625;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		float AttackRange = 120;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Moving")
		float SprintSpeed = 600.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Moving")
		float RunSpeed = 400.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Moving")
		float WalkSpeed = 200.0f;

public:
	FORCEINLINE float GetMaxHealth() { return MaxHealth; }
	FORCEINLINE float GetHealth() { return Health; }
	FORCEINLINE float GetMaxMp() { return MaxMp; }
	FORCEINLINE float GetMp() { return Mp; }
	FORCEINLINE float GetAttackDamage() { return AttackDamage; }
	FORCEINLINE float GetAbilityPower() { return AbilityPower; }
	FORCEINLINE float GetAttackSpeed() { return AttackSpeed; }
	FORCEINLINE float GetAttackRange() { return AttackRange; }

	FORCEINLINE float GetSprintSpeed() { return SprintSpeed; }
	FORCEINLINE float GetRunSpeed() { return RunSpeed; }
	FORCEINLINE float GetWalkSpeed() { return WalkSpeed; }

	FORCEINLINE bool CanMove() { return bCanMove; }

public:	
	UCStatusComponent();

	void AddHealth(float InValue);
	void SubHealth(float InValue);
	FORCEINLINE void ResetHealth() { Health = MaxHealth; }
	FORCEINLINE void SetHealth(float InValue) { Health = InValue; }

	void SetMoveMode();
	void SetStopMode();

	void LoadingCoolTime(float tick);

	void ResetStatus();

protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(BlueprintReadOnly)
		float Health;
	UPROPERTY(BlueprintReadOnly)
		float Mp;
	UPROPERTY(BlueprintReadOnly)
		float CoolTime;

private:
	bool bCanMove = true;

};
