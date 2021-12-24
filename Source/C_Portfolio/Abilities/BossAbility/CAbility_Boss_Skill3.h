#pragma once

#include "CoreMinimal.h"
#include "Abilities/CAbility_Enemy.h"
#include "CAbility_Boss_Skill3.generated.h"

UCLASS()
class C_PORTFOLIO_API ACAbility_Boss_Skill3 : public ACAbility_Enemy
{
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly)
		float JumpPower;
		
	UPROPERTY(EditDefaultsOnly)
		float DownPower;

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class ACBossSkill3_Middle_Object> ObjectClass;
	
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

public:
	virtual void Behavior() override;
	virtual void Begin_Behavior() override;
	virtual void Middle_Behavior() override;
	virtual void Special_Behavior() override;

	virtual void End_Behavior() override;

	virtual void OnColliders() override;
	virtual void OffColliders() override;

	virtual void SkillPreview() override;

	virtual void Cancel() override;
	//virtual float GetPower() override;

	UFUNCTION(BlueprintCallable) //Ability로 옮길것
		void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);


public:
	ACAbility_Boss_Skill3();

private:

	class ACEnemy_Boss* OwnerBoss;
	class ACBossSkill3_Middle_Object* Object;
};
