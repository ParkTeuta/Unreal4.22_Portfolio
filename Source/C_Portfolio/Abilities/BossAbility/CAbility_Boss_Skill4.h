#pragma once

#include "CoreMinimal.h"
#include "Abilities/CAbility_Enemy.h"
#include "CAbility_Boss_Skill4.generated.h"

UCLASS()
class C_PORTFOLIO_API ACAbility_Boss_Skill4 : public ACAbility_Enemy
{
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class AActor> RangeClass;

	UPROPERTY(EditDefaultsOnly)
		class UParticleSystem* GaugeParticle;

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class ACBossSkill4_ThrowItem> ThrowItemClass;
	
public:
	UPROPERTY(BlueprintReadOnly)
		class AActor* RangeObject;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

public:
	virtual void Behavior() override;
	virtual void Begin_Behavior() override;
	virtual void Middle_Behavior() override;

	virtual void End_Behavior() override;

	virtual void OnColliders() override;
	virtual void OffColliders() override;

	virtual void SkillPreview() override;

	virtual void Cancel() override;
	//virtual float GetPower() override;

	UFUNCTION(BlueprintImplementableEvent)
		TArray<FTransform> GetRangeTransforms();

	UFUNCTION(BlueprintCallable) //Ability로 옮길것
		void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);


public:
	ACAbility_Boss_Skill4();

private:

	class ACEnemy_Boss* OwnerBoss;

	float ColorValue;
	bool bColorChange;

	class UParticleSystemComponent* Particle;
};
