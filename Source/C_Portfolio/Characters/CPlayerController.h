#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Components/CSkillComponent.h"
#include "CPlayerController.generated.h"

UCLASS()
class C_PORTFOLIO_API ACPlayerController : public APlayerController
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleDefaultsOnly)
		class UInputComponent* Input;

public:
	FORCEINLINE class  UInputComponent* GetInput() { return Input; }

public:
	ACPlayerController();
	
protected:
	uint32 bMoveToMouseCursor : 1;

	virtual void PlayerTick(float DeltaTime) override;

	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* aPawn) override;
	virtual void OnUnPossess() override;
	virtual void SetupInputComponent() override;

private: //Bind Axis
		//Widget
	UPROPERTY(EditAnywhere)
		TSubclassOf<class UCWidget_PlayerUI> PlayerUIClass;
	class UCWidget_PlayerUI* PlayerUI;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class UCWidget_Dialogue> DialogueClass;
	class UCWidget_Dialogue* DialogueUI;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class UUCWidget_ElapsedBar> InteractionBarClass;
	class UUCWidget_ElapsedBar* InteractionBar;

protected: //Bind Action
	/** Navigate player to the current mouse cursor location. */
	void MoveToMouseCursor();

	/** Navigate player to the given world location. */
	void SetNewMoveDestination(const FVector DestLocation);

	/** Input handlers for SetDestination action. */
	void OnSetDestinationPressed();
	void OnSetDestinationReleased();

	//UseAction
	void OnUseActionPressed();

	//SkillButton
	void OnQSkillPressed();
	void OnWSkillPressed();
	void OnESkillPressed();
	void OnRSkillPressed();
	void OnASkillPressed();
	void OnSSkillPressed();
	void OnDSkillPressed();
	void OnFSkillPressed();
	void OnZSkillPressed();
	void OnDashSkillPressed();

	void SkillPressed(ESkillButtonType InButton);

	void InteractionPressed();

	void OnQSkillReleased();
	void OnWSkillReleased();
	void OnESkillReleased();
	void OnRSkillReleased();
	void OnASkillReleased();
	void OnSSkillReleased();
	void OnDSkillReleased();
	void OnFSkillReleased();
	void OnZSkillReleased();
	void OnDashSkillReleased();


	//Input SpaceBar
	void OnSetHoldingCameraPressed();
	void OnSetHoldingCameraReleased();

	//Input Zoom
	void ZoomInOut(float Axis);

private:
	class ACPlayer* Player;
	class UCStateComponent* State;
	class UCStatusComponent* Status;

	bool Holding = false;
};
