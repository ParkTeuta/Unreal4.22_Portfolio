#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/DataTable.h"
#include "Components/CStateComponent.h"
#include "CMontagesComponent.generated.h"

//=============================================================================
UENUM(BlueprintType)
enum class EMontageType : uint8
{
	None, Attack, Skill, Dead, Dash, Hitted, Equip, Unequip,
};

//=============================================================================
USTRUCT(BlueprintType)
struct FMontageData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		EMontageType MontageType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		class UAnimMontage* AnimMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float PlayRatio = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FName StartSection;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		bool bCanMove;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float Power;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float HitStop;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		class UParticleSystem* HitEffect;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float CoolTime;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float Range;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float TargetCount;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float Radius = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		class UPaperSprite* Sprite;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		class USoundBase* HitSound;



};
//=============================================================================
//PaperSprite'/Game/Imgae/SkillImg4/SkillImg4_Sprite_1.SkillImg4_Sprite_1'

USTRUCT(BlueprintType)
struct FMontageData_Character : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		class UAnimMontage* AnimMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float PlayRatio = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FName StartSection;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		bool bCanMove;
};

//=============================================================================

USTRUCT(BlueprintType)
struct FMontageData_Equip : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		class UAnimMontage* AnimMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float PlayRatio = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FName StartSection;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		bool bCanMove;
};

//=============================================================================


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class C_PORTFOLIO_API UCMontagesComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = "DataTable")
		class UDataTable* DataTable;

public:	
	UCMontagesComponent();

	void MontagesDataReload();
	//const FMontageData* GetSkillData(EMontageType InMontageType);

	//UFUNCTION(BlueprintCallable)

	const FMontageData* GetAttackData(int32 InIndex);
	TArray<FMontageData>* GetAttackDatas();
	const FMontageData* GetAttackData();
	//UFUNCTION(BlueprintCallable)
	const FMontageData* GetSkillData();

	const FMontageData_Character* GetDashData();

	const FMontageData_Character* GetHittedData();
	const FMontageData_Character* GetDeadData();

	const FMontageData_Equip* GetEquipData();
	const FMontageData_Equip* GetUnequipData();

protected:
	virtual void BeginPlay() override;

private:
	void SetAttackData(const FMontageData* InData);
	void SetSkillData(const FMontageData* InData);
	
	void SetDashData(const FMontageData* InData);

	void SetHittedData(const FMontageData* InData);
	void SetDeadData(const FMontageData* InData);

	void SetEquipData(const FMontageData* InData);
	void SetUnequipData(const FMontageData* InData);

	void AddAttackData(const FMontageData* InData);

private:
	TArray<FMontageData *> MontageDatas;

	TArray<FMontageData> AttackDatas;

	FMontageData AttackData;
	FMontageData SkillData;

	FMontageData_Equip EquipData;
	FMontageData_Equip UnequipData;

	FMontageData_Character DashData;

	FMontageData_Character HittedData;

	FMontageData_Character DeadData;
};
