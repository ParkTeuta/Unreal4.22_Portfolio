#pragma once

#include "CoreMinimal.h"

#define CURRENT_LINE CLog::Log(__FUNCTION__, __LINE__);
#define Check(expression, msg) if(!expression){ CLog::LogError(__FUNCTION__, __LINE__,msg); return;}
#define CheckResult(expression, msg, result) if(!expression){ CLog::LogError(__FUNCTION__, __LINE__,msg); return result;} //return ¿÷¿ª ∂ß

class C_PORTFOLIO_API CLog
{
public:
	static void Print(int32 InValue, int32 InKey = -1, float InDuration = 10.0f, FColor InColor = FColor::Blue);
	static void Print(float InValue, int32 InKey = -1, float InDuration = 10.0f, FColor InColor = FColor::Blue);
	static void Print(const FString& InValue, int32 InKey = -1, float InDuration = 10.0f, FColor InColor = FColor::Blue);
	static void Print(const FVector& InValue, int32 InKey = -1, float InDuration = 10.0f, FColor InColor = FColor::Blue);
	static void Print(const FRotator& InValue, int32 InKey = -1, float InDuration = 10.0f, FColor InColor = FColor::Blue);

	static void Log(int32 InValue);
	static void Log(float InValue);
	static void Log(const FString& InValue);
	static void Log(const FVector& InValue);
	static void Log(const FRotator& InValue);
	static void Log(const UObject* InObject);
	static void Log(const FString& InFuncName, int32 InLineNumber);

	static void LogError(const FString& InFuncName, int32 InLineNumber, FString InMessage = "");
};
