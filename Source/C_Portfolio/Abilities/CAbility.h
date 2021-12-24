#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/CMontagesComponent.h"
#include "Components/CSkillComponent.h"
#include "CAbility.generated.h"


UCLASS()
class C_PORTFOLIO_API ACAbility : public AActor //각 스킬들은 ACAbility를 상속받아 기능을 구현한다.
{
	GENERATED_BODY()


public:
	ACAbility();

	//스킬 사용 시 필요한 정보를 담은 구조체 FMontageData / 몽타쥬 관련 값과 스킬과 관련된 가변성 있는 데이터 모음.
	FORCEINLINE const FMontageData* GetMontageData() { return MontageData; } 
	FORCEINLINE void SetMontagesData(const FMontageData* InMontageData) { MontageData = InMontageData; }

	FORCEINLINE const TArray<FMontageData>* GetMontagesDatas() { return MontageDatas; }
	FORCEINLINE void SetMontagesDatas(const TArray<FMontageData>* InMontageDatas) { MontageDatas = InMontageDatas; }

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

public:
	virtual void Behavior() {}; //스킬 시작. 보통 스킬 몽타쥬를 재생함. 

	//몽타쥬 내에서 노티파이로 호출되는 함수들. 필요에 따라 오버라이드 해서 사용함.
	virtual void Begin_Behavior() {}; 
	virtual void Middle_Behavior() {};
	virtual void Special_Behavior() {};
	virtual void End_Behavior() {}; //스킬이 완전히 끝났을 때 호출 (노티파이에서 모션이 끝나기 전에 불러준다.)

	//스킬에 투사체가 붙어있는 경우(근거리)에 사용.
	virtual void OnColliders() {};
	virtual void OffColliders() {};

	//스킬 미리보기
	virtual void SkillPreview() {};

	//노티파이로 호출됨. 스킬에서 세팅한 값만큼 앞으로 밀어줌.
	virtual void PushForward();

	//스킬 취소
	virtual void Cancel();

	//스킬 데미지
	virtual float GetPower() { return MontageData->Power; }
	virtual float GetPower(int32 InCount) { if (MontageDatas->Num() <= InCount) return 0; return MontageDatas->GetData()[InCount].Power; }

	//중복된 캐릭터는 때리지 않도록 하는 함수.
	bool ExistHitActor(class ACharacter* InCharacter);

	//아예 때리는 게 예외되는 캐릭터들 
	virtual bool ExceptionCharacter(class ACharacter* InCharacter);

	//만약 콤보로 이어지는 동작일 시 사용.
	FORCEINLINE void OnNextCombo() { bEnable = true; }
	FORCEINLINE void OffNextCombo() { bEnable = false; }
	FORCEINLINE int32 GetCount() { return Count; }


	FORCEINLINE bool GetbStop() { return bStop; }
	FORCEINLINE SkillInputType GetSkillInputType() { return InputType; }
	FORCEINLINE SkillPreviewType GetSkillPreviewType() { return PreviewType; }

public:
	UFUNCTION(BlueprintPure)
		class ACPlayer* GetOwnerPlayer();

protected:
	UPROPERTY(EditDefaultsOnly)
		float PushForwardPower;

protected:
	class UCStateComponent* State;
	class UCStatusComponent* Status;
	const FMontageData* MontageData;
	const TArray<FMontageData>* MontageDatas;
	class ACSkill* OwnerSkill;
	class ACharacter* OwnerCharacter;

	SkillInputType InputType;
	SkillPreviewType PreviewType = SkillPreviewType::Circle;

	bool bStop;

	bool bReady;
	bool bRangeCheck;

	bool bExist;
	bool bEnable;
	bool bLast;

	int32 Count = 0;
;

	TArray<class ACharacter*> HitActors;
};
