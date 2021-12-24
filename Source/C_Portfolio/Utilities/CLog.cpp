#include "CLog.h"
#include "Engine.h"

DEFINE_LOG_CATEGORY_STATIC(GameProject, Display, All)

void CLog::Print(int32 InValue, int32 InKey, float InDuration, FColor InColor)
{
	GEngine->AddOnScreenDebugMessage(InKey, InDuration, InColor, FString::FromInt(InValue));
}

void CLog::Print(float InValue, int32 InKey, float InDuration, FColor InColor)
{
	GEngine->AddOnScreenDebugMessage(InKey, InDuration, InColor, FString::SanitizeFloat(InValue));
}

void CLog::Print(const FString& InValue, int32 InKey, float InDuration, FColor InColor)
{
	GEngine->AddOnScreenDebugMessage(InKey, InDuration, InColor, InValue);
}

void CLog::Print(const FVector& InValue, int32 InKey, float InDuration, FColor InColor)
{
	GEngine->AddOnScreenDebugMessage(InKey, InDuration, InColor, InValue.ToString());
}

void CLog::Print(const FRotator& InValue, int32 InKey, float InDuration, FColor InColor)
{
	GEngine->AddOnScreenDebugMessage(InKey, InDuration, InColor, InValue.ToString());
}

void CLog::Log(int32 InValue)
{
	UE_LOG(GameProject/*Category name*/, Display, L"%d", InValue);//��ũ��
}

void CLog::Log(float InValue)
{
	UE_LOG(GameProject, Display, L"%f", InValue);//��ũ��
}

void CLog::Log(const FString & InValue)
{
	UE_LOG(GameProject, Display, L"%s", *InValue);//��ũ��
}

void CLog::Log(const FVector & InValue)
{
	UE_LOG(GameProject, Display, L"%s", *InValue.ToString());//��ũ��
}

void CLog::Log(const FRotator & InValue)
{
	UE_LOG(GameProject, Display, L"%s", *InValue.ToString());//��ũ��
}

void CLog::Log(const UObject * InObject)
{
	FString str = "";
	if (InObject != NULL)
		str.Append(InObject->GetName());

	str.Append(InObject != NULL ? "NotNull" : "NULL");
	UE_LOG(GameProject, Display, L"%s", *str);//��ũ��
}

void CLog::Log(const FString & InFuncName, int32 InLineNumber)
{
	FString str = "";
	str.Append(InFuncName);
	str.Append(", ");
	str.Append(FString::FromInt(InLineNumber));

	UE_LOG(GameProject, Display, L"%s", *str);//��ũ��
}

void CLog::LogError(const FString & InFuncName, int32 InLineNumber, FString InMessage)
{
	FString str = "";
	str.Append(InMessage);
	str.Append(", ");
	
	str.Append(InFuncName);
	str.Append(", ");
	str.Append(FString::FromInt(InLineNumber));

	UE_LOG(GameProject, Error, L"%s", *str);//��ũ��

}
