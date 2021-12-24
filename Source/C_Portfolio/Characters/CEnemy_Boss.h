#pragma once

#include "Interface/CQuestInterface.h"

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/CStateComponent.h"
#include "Components/CCrowdControlComponent.h"
#include "Components/CSkillComponent.h"
#include "CEnemy_Boss.generated.h"


//=============================================================================
USTRUCT(BlueprintType)
struct FPhase
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere)
		float MaxHpPercent = 100.0f;

	UPROPERTY(EditAnywhere)
		float MinHpPercent = 0.0f;

	UPROPERTY(EditAnywhere)
		ESkillButtonType SkillButton;

};

UCLASS()
class C_PORTFOLIO_API ACEnemy_Boss : public ACharacter , public ICQuestInterface
{
	GENERATED_BODY()


private:
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class UCWidget_HpBar> HpBarClass;
	/*UPROPERTY(VisibleDefaultsOnly)
		class UCOptionComponent* Option;*/

	UPROPERTY(VisibleDefaultsOnly)
		class UCStateComponent* State;

	UPROPERTY(VisibleDefaultsOnly)
		class UCCrowdControlComponent* CC;

	UPROPERTY(VisibleDefaultsOnly)
		class UCStatusComponent* Status;

	UPROPERTY(VisibleDefaultsOnly)
		class UCMontagesComponent* Montages;

	UPROPERTY(VisibleDefaultsOnly)
		class UCSkillComponent* Skills;

	UPROPERTY(EditDefaultsOnly)
		TArray<FPhase> Phases;

public:
	//FORCEINLINE UCOptionComponent* GetOption() { return Option; }
	UFUNCTION(BlueprintPure)
		FORCEINLINE UCStateComponent* GetState() { return State; }
	FORCEINLINE UCStatusComponent* GetStatus() { return Status; }

public:
	ACEnemy_Boss();
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

public:
	UFUNCTION(BlueprintPure)
		float GetWalkSpeed();
	UFUNCTION(BlueprintPure)
		float GetRunSpeed();
	UFUNCTION(BlueprintPure)
		float GetSprintSpeed();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

private:
	UFUNCTION()
		void OnStateTypeChanged(EStateType InPrevType, EStateType InNewType);

	UFUNCTION()
		void RestoreColor();
public:
	UFUNCTION(BlueprintPure)
		FORCEINLINE class ACharacter* GetAggro() { return Aggro; }

	UFUNCTION(BlueprintCallable)
		void SetAggro(ACharacter* InCharacter);

	UFUNCTION(BlueprintCallable)
		void Attack();
	//void Attack_Implementation();

	// ICQuestInterface을(를) 통해 상속됨
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Get Object Name")
		FString GetObjectName();
		virtual FString GetObjectName_Implementation() override;

	UFUNCTION(BlueprintCallable)
		void SetEnemyName(FString InName);

	UPROPERTY(EditAnywhere)
		FString EnemyName;

	UFUNCTION(BlueprintCallable)
		void UseRandomSkill();

	UFUNCTION(BlueprintPure)
		bool CanSkillUse();

	UFUNCTION(BlueprintCallable)
		void OpenHpWidget();

	UFUNCTION(BlueprintCallable)
		void CloseHpWidget();

	TArray<ESkillButtonType> GetValidList();

	UFUNCTION(BlueprintPure)
		FORCEINLINE FVector GetSpawnLocation() { return SpawnLocation; }

private:
	void Hitted();

	void Dead();
	void End_Dead();

	void Resurrection();

private:
	class UMaterialInstanceDynamic* BodyMaterial;
	class UMaterialInstanceDynamic* BladeMaterial;
	class UMaterialInstanceDynamic* Body2Material;

public:
	void SetBodyColor(FLinearColor InColor);
	void SetBladeColor(FLinearColor InColor);
	void SetBody2Color(FLinearColor InColor);


private:
	FLinearColor BodyColor;
	FLinearColor LogoColor;

	//Damage 입었을 때 전용
	class AController* DamageInstigator;
	float DamageValue;
	float DamageKnockBackPower = 300.0f;
	ECrowdControl DamageCC;
	float DamageCCTime;

	class ACharacter* Aggro;

	FVector SpawnLocation;

	FTimerHandle timer;

	class UCWidget_HpBar* HpBar;
};

