#pragma once

#include "CoreMinimal.h"
#include "Abilities/CAbility_Enemy.h"
#include "CAbility_Boss_Skill2.generated.h"

UCLASS()
class C_PORTFOLIO_API ACAbility_Boss_Skill2 : public ACAbility_Enemy
{
	GENERATED_BODY()

private:
	//UPROPERTY(EditDefaultsOnly)
		
	
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

public:
	virtual void Behavior() override;
	virtual void Begin_Behavior() override;

	virtual void End_Behavior() override;

	virtual void OnColliders() override;
	virtual void OffColliders() override;

	virtual void SkillPreview() override;

	virtual void Cancel() override;
	//virtual float GetPower() override;

	UFUNCTION(BlueprintCallable) //Ability로 옮길것
		void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);


public:
	ACAbility_Boss_Skill2();

private:

	class ACEnemy_Boss* OwnerBoss;
};
