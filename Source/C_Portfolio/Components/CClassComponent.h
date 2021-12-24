#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CClassComponent.generated.h"

UENUM(BlueprintType)
enum class EClassType : uint8
{
	Unarmed, Magic, Fist, Sword, Tutorial
};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FClassTypeChanged, EClassType, InPrevType, EClassType, InNewType);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class C_PORTFOLIO_API UCClassComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly, Category = "Class")
		class TSubclassOf<class ACClass> MagicClass;

	UPROPERTY(EditDefaultsOnly, Category = "Class")
		class TSubclassOf<class ACClass> FistClass;

	UPROPERTY(EditDefaultsOnly, Category = "Class")
		class TSubclassOf<class ACClass> SwordClass;

	UPROPERTY(EditDefaultsOnly, Category = "Class")
		class TSubclassOf<class ACClass> TutorialClass;

public:
	UFUNCTION(BlueprintPure, BlueprintCallable)
		FORCEINLINE EClassType GetType() { return Type; }

	UFUNCTION(BlueprintPure, BlueprintCallable)
		FORCEINLINE bool IsUnarmed() { return Type == EClassType::Unarmed; }

	UFUNCTION(BlueprintPure, BlueprintCallable)
		FORCEINLINE bool IsMagic() { return Type == EClassType::Magic; }

	UFUNCTION(BlueprintPure, BlueprintCallable)
		FORCEINLINE bool IsFist() { return Type == EClassType::Fist; }

	UFUNCTION(BlueprintPure, BlueprintCallable)
		FORCEINLINE bool IsSword() { return Type == EClassType::Sword; }

public:
	FORCEINLINE class ACClass* GetCClass() { return CClass; }

	class UPaperSprite* GetCClassSprite();

public:	
	UCClassComponent();


protected:
	virtual void BeginPlay() override;

	
public:
	UFUNCTION(BlueprintCallable)
		void SetMode(EClassType InNewType);

private:
	void SetUnarmedMode();
	void SetMagicMode();
	void SetFistMode();
	void SetSwordMode();

	void SetTutorialMode();

private:
	void ChangeType(EClassType InNewType);

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
		FClassTypeChanged OnClassTypeChanged;

private:
	EClassType Type;


private:
	class ACClass* CClass;
	class ACClass* Magic;
	class ACClass* Fist;
	class ACClass* Sword;
	class ACClass* Tutorial;
		
};
