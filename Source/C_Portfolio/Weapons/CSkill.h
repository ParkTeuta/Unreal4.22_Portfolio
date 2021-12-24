#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/CMontagesComponent.h"
#include "Components/CSkillComponent.h"
#include "Components/CCrowdControlComponent.h"
#include "CSkill.generated.h"

//DECLARE_DYNAMIC_MULTICAST_DELEGATE()

UCLASS(Abstract)
class C_PORTFOLIO_API ACSkill : public AActor
{
	GENERATED_BODY()


	
private:
	UPROPERTY(EditDefaultsOnly, Category = "Ability")
		TSubclassOf<class ACAbility> AbilityClass;

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = "Camera", meta = (AllowPrivateAccess = true))
		class UDecalComponent* CursorToWorld;

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = "Camera", meta = (AllowPrivateAccess = true))
		class UDecalComponent* ArrowToWorld;

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = "Camera", meta = (AllowPrivateAccess = true))
		class UDecalComponent* RangeToWorld;

protected:
	UPROPERTY(EditDefaultsOnly)
		float KnockBackPower;

	UPROPERTY(EditDefaultsOnly)
		ECrowdControl CC;

	UPROPERTY(EditDefaultsOnly)
		float CCTime;

	UPROPERTY(VisibleDefaultsOnly)
		class UCMontagesComponent* Montages;

	UPROPERTY(VisibleDefaultsOnly)
		class ACThrowItem* ThrowItem;

public:
	FORCEINLINE class UCMontagesComponent* GetMontage() { return Montages; }
	FORCEINLINE class UCStateComponent* GetState() { return State; }
	FORCEINLINE class UCStatusComponent* GetStatus() { return Status; }
	FORCEINLINE class UCSkillComponent* GetSkillComp() { return SkillComp; }
	//FORCEINLINE class UCCombatComponent* GetCombat() { return Combat; }

	FORCEINLINE class ACAbility* GetAbility() { return Ability; }
	FORCEINLINE class ACharacter* GetOwnerCharacter() { return OwnerCharacter; }

	FORCEINLINE float GetPower() { return Power; }
	void SetPower(float InPower) { Power = InPower; }
	class USoundBase* GetHitSound();

	SkillInputType GetInputType();
public:
	static class ACSkill* Spawn(class TSubclassOf<ACSkill> InClass, class UWorld* InWorld, class ACharacter* InCharacter);

public:	
	ACSkill();

	void Behavior();

	bool GetbStop();


protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
		void AttachToOwnerMesh(FName InSocket);

	UFUNCTION(BlueprintCallable)
		void AttachComponentToOwnerMesh(class USceneComponent* InComponent, FName InSocket);

public:	
	virtual void Tick(float DeltaTime) override;
	void Play(class UAnimMontage* InMontage, float InPlayRatio = 1.0f, FName StartSectionName = "");

	void GetShapeComponent(TArray<class UShapeComponent*>& OutComponents);

	UFUNCTION(BlueprintCallable) //Ability로 옮길것
		void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION()
		void OnRestoreTimer();

public:
	void SkillUse();
	void SkillPreview();
	void Cancel();

public:
	//UFUNCTION(BlueprintPure, BlueprintCallable)
	FORCEINLINE	void SetbReady(bool InbReady) { bReady = InbReady; }
	//UFUNCTION(BlueprintPure, BlueprintCallable)
	FORCEINLINE	bool GetbReady() { return bReady; }

	FORCEINLINE void SetSkillCool() { SkillResetTime = SkillCoolTime; }

	FORCEINLINE void SetRange(float InRange) { Range = InRange; }
	FORCEINLINE float GetRange() { return Range; }

	FORCEINLINE void SetbSkillRange(bool InBool) { bSkillRange = InBool; }
	FORCEINLINE bool GetbSkillRange() { return bSkillRange; }

	FORCEINLINE int32 GetSkillRemainCoolTime() { return SkillResetTime; }

	FORCEINLINE float GetKnockBackPower() { return KnockBackPower; }
	FORCEINLINE void SetKnockBackPower(float InPower) { KnockBackPower = InPower; }

	FORCEINLINE ECrowdControl GetCC() { return CC; }
	FORCEINLINE float GetCCTime() { return CCTime; }

	SkillInputType GetSkillInputType();

	ACharacter* GetEnemy();

	FORCEINLINE ESkillButtonType GetSkillButton() { return SkillButton; }
	FORCEINLINE void SetSkillButton(ESkillButtonType InType) { SkillButton = InType; }

	//void SetMontageType(EMontageType  InMontageType);
//collider
public:
	TArray<class UShapeComponent*> GetClassColliders();
	void SetClassColliders(TArray<class UShapeComponent*> InComponents);

	int32 GetSoundCount() { return SoundCount; }
	void SetSoundCount(int32 InSoundCount) { SoundCount = InSoundCount; }

protected:
	UPROPERTY(BlueprintReadOnly)
		class ACharacter* OwnerCharacter;

	UPROPERTY(BlueprintReadOnly)
		class UCStateComponent* State;

	UPROPERTY(BlueprintReadOnly)
		class UCStatusComponent* Status;

	UPROPERTY(BlueprintReadOnly)
		class UCSkillComponent* SkillComp;

	ESkillButtonType SkillButton;

	class ACAbility* Ability;

	//EMontageType MontageType;

	TArray<class UShapeComponent*> ClassColliders;

	float Power = 10.0f;
	float Range =150.0f;
	float Radius;
	float SkillCoolTime;
	float SkillResetTime=0.0f;
	bool bReady = true; 
	bool bSkillRange = false;

	int32 SoundCount = 0;

};
