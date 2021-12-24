#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CClass.generated.h"

UCLASS()
class C_PORTFOLIO_API ACClass : public AActor
{
	GENERATED_BODY()
	
public:	
	ACClass();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditDefaultsOnly)
		FString ClassName;

protected:
	UPROPERTY(VisibleDefaultsOnly)
		class UCMontagesComponent* Montages;

public:
	FORCEINLINE class UCMontagesComponent* GetMontage() { return Montages; }
	FORCEINLINE class UCStateComponent* GetState() { return State; }
	FORCEINLINE class UCSkillComponent* GetSkills() { return Skills; }
	FORCEINLINE class ACharacter* GetOwnerCharacter() { return OwnerCharacter; }
	FORCEINLINE bool GetEquip() { return bEquip; }

private: //Class Skill
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

	UPROPERTY(EditDefaultsOnly, Category = "Class")
		class UPaperSprite* ClassSprite;

public:
	void ChangeClass();

	UFUNCTION(BlueprintNativeEvent)
		void Equip();
	void Equip_Implementation();

	UFUNCTION(BlueprintNativeEvent)
		void Begin_Equip();
	void Begin_Equip_Implementation();

	UFUNCTION(BlueprintNativeEvent)
		void End_Equip();
	void End_Equip_Implementation();

	UFUNCTION(BlueprintNativeEvent)
		void Unequip();
	void Unequip_Implementation();

	FORCEINLINE class UPaperSprite* GetClassSprite() { return ClassSprite; }


protected:
	UPROPERTY(BlueprintReadOnly)
		class ACharacter* OwnerCharacter;

	UPROPERTY(BlueprintReadOnly)
		class UCSkillComponent* Skills;

	UPROPERTY(BlueprintReadOnly)
		class UCStateComponent* State;

public:	
	UFUNCTION(BlueprintCallable)
		void AttachToOwnerMesh(FName InSocket);

	UFUNCTION(BlueprintCallable)
		void AttachComponentToOwnerMesh(class USceneComponent* InComponent, FName InSocket);

	virtual void Tick(float DeltaTime) override;
	void Play(class UAnimMontage* InMontage, float InPlayRatio = 1.0f, FName StartSectionName = "");

	void GetShapeComponent(TArray<class UShapeComponent*>& OutComponents);

private:
	class ACSkill* Attack;

	class ACSkill* QSkill;
	class ACSkill* WSkill;
	class ACSkill* ESkill;
	class ACSkill* RSkill;
	class ACSkill* ASkill;
	class ACSkill* SSkill;
	class ACSkill* DSkill;
	class ACSkill* FSkill;
	class ACSkill* DashSkill;

	bool bEquip;
};
