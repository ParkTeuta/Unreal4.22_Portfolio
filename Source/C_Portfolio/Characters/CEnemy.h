#pragma once

#include "Interface/CQuestInterface.h"

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/CStateComponent.h"
#include "Components/CCrowdControlComponent.h"
#include "Components/CSkillComponent.h"
#include "CEnemy.generated.h"

UCLASS()
class C_PORTFOLIO_API ACEnemy : public ACharacter , public ICQuestInterface
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleDefaultsOnly)
		class UTextRenderComponent* NameText;

	UPROPERTY(VisibleDefaultsOnly)
		class UWidgetComponent* HealthBar;

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


public:
	//FORCEINLINE UCOptionComponent* GetOption() { return Option; }
	UFUNCTION(BlueprintPure)
		FORCEINLINE UCStateComponent* GetState() { return State; }
	FORCEINLINE UCStatusComponent* GetStatus() { return Status; }

public:
	ACEnemy();
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

	UFUNCTION(BlueprintCallable)
		void UseSkill(ESkillButtonType InSkillButton);

	// ICQuestInterface��(��) ���� ��ӵ�
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Get Object Name")
		FString GetObjectName();
		virtual FString GetObjectName_Implementation() override;

	UFUNCTION(BlueprintCallable)
		void SetEnemyName(FString InName);

	UPROPERTY(EditAnywhere)
		FString EnemyName;

	UFUNCTION(BlueprintPure)
		FORCEINLINE FVector GetSpawnLocation() { return SpawnLocation; }

private:
	void Hitted();

	void Dead();
	void End_Dead();

	void Resurrection();

private:
	class UMaterialInstanceDynamic* BodyMaterial;
	class UMaterialInstanceDynamic* LogoMaterial;

private:
	FLinearColor BodyColor;
	FLinearColor LogoColor;

	//Damage �Ծ��� �� ����
	class AController* DamageInstigator;
	float DamageValue;
	float DamageKnockBackPower = 300.0f;
	ECrowdControl DamageCC;
	float DamageCCTime;

	class ACharacter* Aggro;

	FVector SpawnLocation;

	FTimerHandle timer;
};

