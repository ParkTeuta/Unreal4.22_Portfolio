#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/CMontagesComponent.h"
#include "Components/CSkillComponent.h"
#include "CAbility.generated.h"


UCLASS()
class C_PORTFOLIO_API ACAbility : public AActor //�� ��ų���� ACAbility�� ��ӹ޾� ����� �����Ѵ�.
{
	GENERATED_BODY()


public:
	ACAbility();

	//��ų ��� �� �ʿ��� ������ ���� ����ü FMontageData / ��Ÿ�� ���� ���� ��ų�� ���õ� ������ �ִ� ������ ����.
	FORCEINLINE const FMontageData* GetMontageData() { return MontageData; } 
	FORCEINLINE void SetMontagesData(const FMontageData* InMontageData) { MontageData = InMontageData; }

	FORCEINLINE const TArray<FMontageData>* GetMontagesDatas() { return MontageDatas; }
	FORCEINLINE void SetMontagesDatas(const TArray<FMontageData>* InMontageDatas) { MontageDatas = InMontageDatas; }

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

public:
	virtual void Behavior() {}; //��ų ����. ���� ��ų ��Ÿ�긦 �����. 

	//��Ÿ�� ������ ��Ƽ���̷� ȣ��Ǵ� �Լ���. �ʿ信 ���� �������̵� �ؼ� �����.
	virtual void Begin_Behavior() {}; 
	virtual void Middle_Behavior() {};
	virtual void Special_Behavior() {};
	virtual void End_Behavior() {}; //��ų�� ������ ������ �� ȣ�� (��Ƽ���̿��� ����� ������ ���� �ҷ��ش�.)

	//��ų�� ����ü�� �پ��ִ� ���(�ٰŸ�)�� ���.
	virtual void OnColliders() {};
	virtual void OffColliders() {};

	//��ų �̸�����
	virtual void SkillPreview() {};

	//��Ƽ���̷� ȣ���. ��ų���� ������ ����ŭ ������ �о���.
	virtual void PushForward();

	//��ų ���
	virtual void Cancel();

	//��ų ������
	virtual float GetPower() { return MontageData->Power; }
	virtual float GetPower(int32 InCount) { if (MontageDatas->Num() <= InCount) return 0; return MontageDatas->GetData()[InCount].Power; }

	//�ߺ��� ĳ���ʹ� ������ �ʵ��� �ϴ� �Լ�.
	bool ExistHitActor(class ACharacter* InCharacter);

	//�ƿ� ������ �� ���ܵǴ� ĳ���͵� 
	virtual bool ExceptionCharacter(class ACharacter* InCharacter);

	//���� �޺��� �̾����� ������ �� ���.
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
