#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/CStateComponent.h"
#include "Components/CSkillComponent.h"
#include "Components/CRecipientComponent.h"
#include "Components/CClassComponent.h"
#include "CPlayer.generated.h"

UCLASS()
class C_PORTFOLIO_API ACPlayer : public ACharacter
{
	GENERATED_BODY()

public:
	ACPlayer();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditDefaultsOnly, Category = "CCamera")
		TSubclassOf<class ACCamera> CCameraClass;

public:
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
		class USpringArmComponent* SpringArm;
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
		class UCameraComponent* Camera;

private:
	UPROPERTY(VisibleDefaultsOnly)
		class ACCamera* FreeCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UDecalComponent* CursorToWorld;



	//Player Components
	UPROPERTY(VisibleDefaultsOnly) 
		class UCStateComponent* State;

	UPROPERTY(VisibleDefaultsOnly)
		class UCStatusComponent* Status;

	UPROPERTY(VisibleDefaultsOnly)
		class UCMontagesComponent* Montages;

	UPROPERTY(VisibleDefaultsOnly) //skill 종류 관리
		class UCSkillComponent* Skills;

	UPROPERTY(VisibleDefaultsOnly) //직업
		class UCClassComponent* CClasses;

	UPROPERTY(VisibleDefaultsOnly) //일단 에어본만 있음
		class UCCrowdControlComponent* CC;

	UPROPERTY(VisibleDefaultsOnly)
		class UCMoveInteractionComponent* MoveInteractionComp;
public:
	UPROPERTY(BlueprintReadWrite, VisibleDefaultsOnly) //진행중인 quest 관리
		class UCRecipientComponent* QuestRecipient;

public:

	UFUNCTION(BlueprintPure)
	FORCEINLINE UCClassComponent* GetCClasses() { return CClasses; }
	UFUNCTION(BlueprintPure)
		FORCEINLINE UCStateComponent* GetState() { return State; }
	UFUNCTION(BlueprintPure)
		FORCEINLINE UCRecipientComponent* GetRecipient() { return QuestRecipient; }

	FORCEINLINE UCStatusComponent* GetStatus() { return Status; }
	FORCEINLINE void SetSkillEnemy(ACharacter* InEnemy) { Skills->SetEnemy(InEnemy); }
	FORCEINLINE void SetTargetPosition(FVector InPosition) { Skills->SetTargetPosition(InPosition); }
	FORCEINLINE ACharacter* GetEnemy() { return Enemy; }
	FORCEINLINE void SetBaseAttack(bool InBaseAttack) { bBaseAttack = InBaseAttack;  }
	FORCEINLINE void AttackCancel() { Skills->AttackCancel(); }
	FORCEINLINE bool GetAttackbStop() { return Skills->GetAttackbStop(); }

	FORCEINLINE bool IsSkillWait() { return !!Skills->GetWaitSkill(); }
	FORCEINLINE class ACSkill* GetSkillWait() { return Skills->GetWaitSkill(); }

	FORCEINLINE void SetCLocation(FVector InLocation) { CLocation = InLocation; }

public:
	UFUNCTION(BlueprintPure, Category = "Sprite")
		class UPaperSprite* GetSprite(ESkillButtonType Input);

	UFUNCTION(BlueprintPure, Category = "Skill")
		bool GetSkillReady(ESkillButtonType Input);

	UFUNCTION(BlueprintPure, Category = "Skill")
		int32 GetSkillCoolTime(ESkillButtonType Input);

	UFUNCTION(BlueprintCallable)
		void SetPlayerInfo(float InHealth, EClassType InClassType);

	UFUNCTION(BlueprintPure)
		float GetHealth();

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

private:
	UFUNCTION()
		void OnStateTypeChanged(EStateType InPrevType, EStateType InNewType);

public:
	//키입력관련
	UFUNCTION(BlueprintPure)
		bool GetSkillsCanUse();
	//스킬이 어떤 타입인지
	void SetSkillButton(ESkillButtonType InNewButton);
	void SetSkillReleased(ESkillButtonType InNewButton);
	bool GetSKillIsHolding(ESkillButtonType InNewButton); //누르고 있어야하는지
	bool GetSKillIsImmediately(ESkillButtonType InNewButton); //즉시시전인지

	void StopMove();

public:
	//InCameraMode: true-Camera , false-FreeCamera;
	void SetCameraMode(bool InCameraMode); 

	void SetCameraZoom(float Axis);

public:
	//Player Action
	void Dead();
	UFUNCTION(BlueprintImplementableEvent)
		void ExtraDead();
	UFUNCTION(BlueprintImplementableEvent)
		void GameBadEnding();


	void SetAttackTarget(ACharacter* InEnemy);
	void MoveToTarget();
	void MoveToTarget(FVector InTargetPosition);
	void Attack();
	void Action();

	void SkillCancel();

	UFUNCTION(BlueprintCallable)
		void OnTutorialClass();
	UFUNCTION(BlueprintCallable)
		void OnMagic();
	UFUNCTION(BlueprintCallable)
		void OnFist();
	UFUNCTION(BlueprintCallable)
		void OnSword();

	void ChangeCClass();

public: //Quest
	UFUNCTION(BlueprintNativeEvent)
		void RefreshQuestList();
		void RefreshQuestList_Implementation();

	UFUNCTION(BlueprintNativeEvent)
		void RefreshQuestItem(int32 InCount);
		void RefreshQuestItem_Implementation(int32 InCount);


	UFUNCTION(BlueprintCallable)
		void AddQuestItem(AActor* InActor, int32 InCount = 1); //When I receive any item, I will process it with this function

	UFUNCTION(BlueprintCallable)
		void SetPlayerData(ACPlayer* InPlayer);

private:
	void SetFreeCameraToCamera();
	void SetFreeCameraToCameraZ();

private:
	ACharacter* Enemy;
	FVector CLocation;

	bool Holding=false;
	bool bBaseAttack = false;

public:
	void SetPlayerUIWidget(class UCWidget_PlayerUI* InWidget);
	void UpdateSkillSprite();
	void UpdateSkillSpriteInSkillButton(ESkillButtonType InSkillButton, class UPaperSprite* InSprite);
	void UpdateClassSprite(class UPaperSprite* InSprite);
	void UpdateHealthBar();

	void SetPlayerUIVisibility(ESlateVisibility InVisibility);

	FORCEINLINE void SetInteractionBar(class UUCWidget_ElapsedBar* InWidget) { InteractionBar = InWidget; }
	FORCEINLINE UUCWidget_ElapsedBar* GetInteractionBar() { return InteractionBar; }

private:
	class UCWidget_PlayerUI* PlayerUIWidget;
	class UUCWidget_ElapsedBar* InteractionBar;
private:

	class AController* DamageInstigator;
	float DamageValue;

public:
	bool CanActionMoveInteraction();
	
	void StartMoveInteraction();
	void EndMoveInteraction();

	UPROPERTY(EditDefaultsOnly)
		float AutoHeal = 5;
	
	UPROPERTY(EditDefaultsOnly)
		float TickTime = 5;

private:
	float TimeCount= 0;

	bool IsFirstStart = true;
};
