#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CClearConditionComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class C_PORTFOLIO_API UCClearConditionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UCClearConditionComponent();

protected:
	virtual void BeginPlay() override;

public:
	FString GetContent();

	UFUNCTION(BlueprintCallable)
		bool CheckSuccess();

	UFUNCTION(BlueprintCallable)
		bool AddCount(AActor* InActor, int32 InCount);

	FORCEINLINE int32 GetCount() { return Count; }

	FORCEINLINE void SetCount(int32 InCount) { Count = InCount; CheckSuccess(); }

public:
	UPROPERTY(EditAnywhere)
		UClass* CheckClass;

	/*UPROPERTY(EditAnywhere)
		TSubclassOf<AActor> CheckActor;*/

	UPROPERTY(EditAnywhere)
		int32 CheckCount;

	UPROPERTY(EditAnywhere)// _Condition_ : ClearCondition ,_Check_ : Need Item ,_Count_ : Need Count
		FString Content; 

	FORCEINLINE bool GetIsClear() { return IsClear; }

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	int32 Count;

	bool IsClear;
};
