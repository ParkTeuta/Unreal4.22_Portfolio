#pragma once

#include "CoreMinimal.h"
#include "Abilities/CAbility_Player.h"
#include "CAbility_Dash_Magic.generated.h"

UCLASS()
class C_PORTFOLIO_API ACAbility_Dash_Magic : public ACAbility_Player
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class ACRange> Range;

	UPROPERTY(EditDefaultsOnly)
		float MoveDistance;
		//class ACThrowItem* ThrowItem;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

public:
	virtual void Behavior() override;
	virtual void Begin_Behavior() override;
	virtual void End_Behavior() override;



	virtual void SkillPreview() override;

	virtual void Cancel() override;
	//virtual float GetPower() override;
	
	bool ExistHitActor(class ACharacter* InCharacter);

public:
	UFUNCTION(BlueprintNativeEvent)
		void Poseable();
		void Poseable_Implementation();

public:
	FORCEINLINE bool HitActorsOverCount() { return HitActors.Num() == Count; }

private:

	int32 Count = 1;

	TArray<class ACharacter*> HitActors;
};
