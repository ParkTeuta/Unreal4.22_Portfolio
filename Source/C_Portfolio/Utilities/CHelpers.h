#pragma once

#include "CoreMinimal.h"
#include "ConstructorHelpers.h"
#include "GameFramework/Actor.h"
#include "Components/PrimitiveComponent.h"
#include "Components/SceneComponent.h"

class C_PORTFOLIO_API CHelpers
{
public:
	template<typename T>
	static void GetAsset(T** OutObject, FString InPath)
	{
		ConstructorHelpers::FObjectFinder<T> asset(*InPath);
		verifyf(asset.Succeeded(), L"asset.Succeeded() is false");

		*OutObject = asset.Object;
	}

	template<typename T>
	static void GetAssetDynamic(T** OutObject, FString InPath)
	{
		T* obj = Cast<T>(StaticLoadObject(T::StaticClass(), NULL, *InPath));
		verifyf(obj != NULL, L"obj!=NULL");

		*OutObject = obj;
	}

	template<typename T>
	static void GetClass(TSubclassOf<T>* OutClass, FString InPath)
	{
		ConstructorHelpers::FClassFinder<T> asset(*InPath);
		verifyf(asset.Succeeded(), L"asset.Succeeded() is false");

		*OutClass = asset.Class;
	}

	template<typename T>
	static T* GetComponent(AActor* InActor)
	{
		return Cast<T>(InActor->GetComponentByClass(T::StaticClass()));
	}

	static void AttachToComponent(AActor* InActor, USceneComponent* InParnet, FName InSocketName)
	{
		InActor->AttachToComponent(InParnet, FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), InSocketName);
	}

	static FRotator YawRotator(FRotator InRotator)
	{
		FRotator rotator;
		rotator = InRotator;
		rotator.Pitch = 0;
		rotator.Roll = 0;
		return rotator;
	}

	static void RotateVectorToDegree(FVector &InVector, float InDegree) //ภ฿ตส
	{
		float degCos = UKismetMathLibrary::DegCos(InDegree);
		float degSin = UKismetMathLibrary::DegSin(InDegree);
		InVector.X = InVector.X * degCos - InVector.Y * degSin;
		InVector.Y = InVector.X * degSin + InVector.Y * degCos;
		InVector.Normalize();
	}

	static void SetCollisionPresetNoCollision(class UPrimitiveComponent* InComp)
	{
		InComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		InComp->SetCollisionObjectType(ECollisionChannel::ECC_WorldStatic);
		InComp->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
		InComp->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
		InComp->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Ignore);
	}

	static void SetCollisionPresetBlockAll(class UPrimitiveComponent* InComp)
	{
		InComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		InComp->SetCollisionObjectType(ECollisionChannel::ECC_WorldStatic);
		InComp->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	}

	static void SetCollisionPresetBlockAllDynamic(class UPrimitiveComponent* InComp)
	{
		InComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		InComp->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
		InComp->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	}

	static void SetCollisionPresetOverlapAll(class UPrimitiveComponent* InComp)
	{
		InComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		InComp->SetCollisionObjectType(ECollisionChannel::ECC_WorldStatic);
		InComp->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	}

	static void SetCollisionPresetOverlapAllDynamic(class UPrimitiveComponent* InComp)
	{
		InComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		InComp->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
		InComp->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	}

	static void SetCollisionPresetIgnoreOnlyPawn(class UPrimitiveComponent* InComp)
	{
		InComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		InComp->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
		InComp->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
		InComp->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
		InComp->SetCollisionResponseToChannel(ECollisionChannel::ECC_Vehicle, ECollisionResponse::ECR_Ignore);
	}

	static void SetCollisionPresetOverlapOnlyPawn(class UPrimitiveComponent* InComp)
	{
		InComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		InComp->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
		InComp->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
		InComp->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
		InComp->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
		InComp->SetCollisionResponseToChannel(ECollisionChannel::ECC_Vehicle, ECollisionResponse::ECR_Overlap);
	}

	static void SetCollisionPresetPawn(class UPrimitiveComponent* InComp)
	{
		InComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		InComp->SetCollisionObjectType(ECollisionChannel::ECC_Pawn);
		InComp->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
		InComp->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Ignore);
	}

	static void SetCollisionPresetCCharacterMesh(class UPrimitiveComponent* InComp)
	{
		InComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		InComp->SetCollisionObjectType(ECollisionChannel::ECC_Pawn);
		InComp->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
		InComp->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Ignore);
		InComp->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
		InComp->SetCollisionResponseToChannel(ECollisionChannel::ECC_Vehicle, ECollisionResponse::ECR_Ignore);
	}

};
