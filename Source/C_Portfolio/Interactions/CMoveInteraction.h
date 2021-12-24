#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CMoveInteraction.generated.h"

//=============================================================================
UENUM(BlueprintType)
enum class EMoveState : uint8
{
	MS_None, MS_Processing, MS_Finish,
};
//=============================================================================

UCLASS()
class C_PORTFOLIO_API ACMoveInteraction : public AActor
{
	GENERATED_BODY()
	
public:	
	ACMoveInteraction();

public:
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
		class USplineComponent* Spline;

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
		class USphereComponent* InteractionArea1;

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
		class USphereComponent* InteractionArea2;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UDecalComponent* AreaDecal1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UDecalComponent* AreaDecal2;
protected:

	UPROPERTY(EditAnywhere)
		bool bReverse;

public:
	UFUNCTION(BlueprintCallable)
		void Area1BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION(BlueprintCallable) 
		void Area2BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION(BlueprintCallable)
		void AreaEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:
	virtual void Start(class ACPlayer* InPlayer) {};
	virtual void Activity(ACPlayer* InPlayer) {};
	virtual void End(ACPlayer* InPlayer) {};

	virtual void NotifyAction(ACPlayer* InPlayer) {};


public:
	FORCEINLINE EMoveState GetMoveState() { return MoveState; }
	FORCEINLINE void SetMoveState(EMoveState InMoveState) { MoveState = InMoveState; }

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

protected:

	int32 StartIndex;
	int32 EndIndex;

	FVector StartLoc;
	FVector EndLoc;

	EMoveState MoveState;
};
