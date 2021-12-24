#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CQuestInterface.generated.h"

UINTERFACE(MinimalAPI)
class UCQuestInterface : public UInterface
{
	GENERATED_BODY()
};

class C_PORTFOLIO_API ICQuestInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Get Object Name")
		FString GetObjectName();
	//virtual FString GetObjectName() = 0;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Object Visibility")
		bool OnVisibility();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Object Visibility")
		bool OffVisibility();

};
