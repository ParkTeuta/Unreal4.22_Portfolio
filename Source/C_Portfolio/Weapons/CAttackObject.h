#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CAttackObject.generated.h"

UCLASS()
class C_PORTFOLIO_API ACAttackObject : public AActor
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleDefaultsOnly)
		class UParticleSystemComponent* ParticleSystem;

	UPROPERTY(EditDefaultsOnly)
		UParticleSystem* HitParticle;
	
public:	
	ACAttackObject();

protected:
	UFUNCTION(BlueprintCallable)
		virtual void BeginOverlap(AActor* InOtherActor, UParticleSystem* InParticle, FVector& InLocation);

	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, BlueprintPure)
		class ACharacter* GetOwnerCharacter();

	UFUNCTION(BlueprintCallable)
		bool ExistHitActor(class ACharacter* InCharacter);

	virtual bool ExceptionCharacter(class ACharacter* InCharacter);

	FORCEINLINE class ACSkill* GetOwnerSkill() { return OwnerSkill; }
	void SetOwnerSkill(class ACSkill* InSkill);

	bool CanDameged(ACharacter* InCharacter);

public:
	UPROPERTY(BlueprintReadWrite)
		int32 Count = 0;

protected:
	class ACSkill* OwnerSkill;

	TArray<class ACharacter*> HitActors;
};
