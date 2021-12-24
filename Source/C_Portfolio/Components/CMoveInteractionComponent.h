#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Interactions/CMoveInteraction.h"
#include "CMoveInteractionComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class C_PORTFOLIO_API UCMoveInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCMoveInteractionComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	class ACMoveInteraction* GetMoveInteraction();
	void SetMoveInteraction(class ACMoveInteraction* InMoveInteraction);

	void Start();
	void End();

	bool CanActionMoveInteraction();

	FORCEINLINE bool GetbMove() { return bMove; }
	FORCEINLINE void GetbMove(bool InbMove) { bMove = InbMove; }


private:
	class ACPlayer* Player;
	class UCStateComponent* State;

	class ACMoveInteraction* MoveInteraction;

	bool bMove;
};
