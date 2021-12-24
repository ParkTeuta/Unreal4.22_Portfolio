#pragma once

#include "CoreMinimal.h"
#include "CAbility.h"
#include "CAbility_Player.generated.h"


UCLASS()
class C_PORTFOLIO_API ACAbility_Player : public ACAbility
{
	GENERATED_BODY()
	
public:	
	ACAbility_Player();


protected:
	virtual void BeginPlay() override;


public:	
	virtual void Tick(float DeltaTime) override;
	
public:
	virtual void Behavior() override {};
	virtual void Begin_Behavior() override {};
	virtual void End_Behavior() override {};

	virtual void OnColliders() override {};
	virtual void OffColliders() override {};

	virtual void SkillPreview() override {};

	virtual void Cancel() override {};


protected:

};
