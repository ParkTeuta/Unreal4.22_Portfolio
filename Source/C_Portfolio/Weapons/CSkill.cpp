#include "CSkill.h"
#include "Global.h"
#include "Characters/CPlayer.h"
#include "Abilities/CAbility.h"
#include "Abilities/CAbility_Targeting.h"
#include "Abilities/CAbility_NearAttack.h"
#include "Components/CStateComponent.h"
#include "Components/CStatusComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/ShapeComponent.h"
#include "Materials/Material.h"
#include "Components/DecalComponent.h"

#include "Characters/CNpc.h"

#include "Sound/SoundBase.h"

ACSkill::ACSkill()
{
	PrimaryActorTick.bCanEverTick = true;

	Montages = CreateDefaultSubobject<UCMontagesComponent>("Montages");

	KnockBackPower = 500.0f;
	CC = ECrowdControl::None;
	CCTime = 0.0f;

	//decal
	CursorToWorld = CreateDefaultSubobject<UDecalComponent>("CursorToWorld");
	CursorToWorld->SetupAttachment(RootComponent);
	UMaterial* DecalMaterialAsset;
	CHelpers::GetAssetDynamic<UMaterial>(&DecalMaterialAsset, "Material'/Game/TopDownCPP/Blueprints/M_Cursor_Decal_NotInner.M_Cursor_Decal_NotInner'");
	if (!!DecalMaterialAsset)
	{
		CursorToWorld->SetDecalMaterial(DecalMaterialAsset);
	}
	CursorToWorld->DecalSize = FVector(16.0f, 32.0f, 32.0f);
	CursorToWorld->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f).Quaternion());

	//Arrow Decal
	ArrowToWorld = CreateDefaultSubobject<UDecalComponent>("ArrowToWorld");
	ArrowToWorld->SetupAttachment(RootComponent);
	UMaterial* DecalMaterialAsset2;
	CHelpers::GetAssetDynamic<UMaterial>(&DecalMaterialAsset2, "Material'/Game/slate/M_left_arrow.M_left_arrow'");
	if (!!DecalMaterialAsset2)
	{
		ArrowToWorld->SetDecalMaterial(DecalMaterialAsset2);
	}
	ArrowToWorld->DecalSize = FVector(16.0f, 32.0f, 32.0f);
	ArrowToWorld->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f).Quaternion());

	//Range Decal
	RangeToWorld = CreateDefaultSubobject<UDecalComponent>("RangeToWorld");
	RangeToWorld->SetupAttachment(RootComponent);
	UMaterial* DecalMaterialAsset3;
	CHelpers::GetAssetDynamic<UMaterial>(&DecalMaterialAsset3, "Material'/Game/TopDownCPP/Blueprints/M_Cursor_Decal.M_Cursor_Decal'");
	if (!!DecalMaterialAsset3)
	{
		RangeToWorld->SetDecalMaterial(DecalMaterialAsset3);
	}
	RangeToWorld->DecalSize = FVector(16.0f, 32.0f, 32.0f);
	RangeToWorld->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f).Quaternion());

}

void ACSkill::BeginPlay()
{
	OwnerCharacter = Cast<ACharacter>(GetOwner());
	State = CHelpers::GetComponent<UCStateComponent>(OwnerCharacter);
	Status = CHelpers::GetComponent<UCStatusComponent>(OwnerCharacter);
	SkillComp = CHelpers::GetComponent<UCSkillComponent>(OwnerCharacter);
	//Montages = CHelpers::GetComponent<UCMontagesComponent>(OwnerCharacter);

	Super::BeginPlay();

	if (AbilityClass != NULL)
	{
		FActorSpawnParameters params;
		params.Owner = this;
		Ability = GetWorld()->SpawnActor<ACAbility>(AbilityClass, params);
	}

	if (Montages != NULL)
	{
		Ability->SetMontagesData(Montages->GetSkillData());
		Ability->SetMontagesDatas(Montages->GetAttackDatas());
		const FMontageData* data = Montages->GetSkillData();
		if (data != NULL)
		{
			Range = data->Range;
			Radius = data->Radius;
			SkillCoolTime = data->CoolTime;
			Power = data->Power;
		}
	}

	CursorToWorld->DecalSize = FVector(Range/2.0f, Range, Range);
	CursorToWorld->SetVisibility(false);
	

	ArrowToWorld->DecalSize = FVector(Range*0.5f , Range*0.5f, Range*0.5f);
	ArrowToWorld->SetVisibility(false);

	RangeToWorld->DecalSize = FVector(Radius, Radius, Radius);
	RangeToWorld->SetVisibility(false);

	if (ThrowItem != NULL)
	{
		
	}
}


USoundBase * ACSkill::GetHitSound()
{
	const TArray<FMontageData>* data = Ability->GetMontagesDatas();

	return data->GetData()[SoundCount].HitSound;
}

SkillInputType ACSkill::GetInputType()
{
	return Ability->GetSkillInputType();
}

ACSkill * ACSkill::Spawn(TSubclassOf<ACSkill> InClass, UWorld * InWorld, ACharacter * InCharacter)
{
	FActorSpawnParameters params;
	params.Owner = InCharacter;

	FTransform transform;
	ACSkill* weapon = Cast<ACSkill>(InWorld->SpawnActor(InClass, &transform, params));

	return weapon;
}

void ACSkill::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!bReady)
	{
		SkillResetTime -= DeltaTime;
		bReady = SkillResetTime <= 0.0f;
	}

	if (CursorToWorld != nullptr && Ability->GetSkillPreviewType() != SkillPreviewType::Arrow)
	{
		CursorToWorld->SetVisibility(bSkillRange);
		CheckNull(OwnerCharacter);
		FVector CursorPos = OwnerCharacter->GetActorLocation();
		CursorPos.Z -= 90;
		CursorToWorld->SetWorldLocation(CursorPos);
	}
	
	if (ArrowToWorld != nullptr && Ability->GetSkillPreviewType() == SkillPreviewType::Arrow)
	{
		ArrowToWorld->SetVisibility(bSkillRange);
		if (APlayerController* PC = Cast<APlayerController>(OwnerCharacter->GetController()))
		{
			FHitResult TraceHitResult;
			PC->GetHitResultUnderCursor(ECC_Visibility, true, TraceHitResult);

			FVector playerPos = OwnerCharacter->GetActorLocation();
			FVector MousePos = TraceHitResult.Location;
			FRotator rot = CHelpers::YawRotator(UKismetMathLibrary::FindLookAtRotation(playerPos, MousePos));
			FVector position = playerPos + (rot.Vector() * Range/2);
			//position.Z -= 90;
			ArrowToWorld->SetWorldLocation(position);
			rot.Pitch = 90.0f;
			ArrowToWorld->SetWorldRotation(rot);

			//CLog::Log(TraceHitResult.Location);
		}
	}

	if (RangeToWorld != nullptr && Ability->GetSkillPreviewType() == SkillPreviewType::Range)
	{
		RangeToWorld->SetVisibility(bSkillRange);
		if (APlayerController* PC = Cast<APlayerController>(OwnerCharacter->GetController()))
		{
			FHitResult TraceHitResult;
			PC->GetHitResultUnderCursor(ECC_Visibility, true, TraceHitResult);

			FVector playerPos = OwnerCharacter->GetActorLocation();
			FVector MousePos = TraceHitResult.Location;
			FRotator rot = CHelpers::YawRotator(UKismetMathLibrary::FindLookAtRotation(playerPos, MousePos));
			FVector position;
			position = MousePos;
			if (FVector::Distance(MousePos, playerPos) >= Range)
				position = playerPos + (rot.Vector() * Range);
			RangeToWorld->SetWorldLocation(position);
			/*rot.Pitch = 90.0f;
			ArrowToWorld->SetWorldRotation(rot);*/

			
		}

	}

}



void ACSkill::Behavior()
{
	//CheckTrue(Combat->IsUnarmed());
	//CheckFalse(State->IsIdle());
	CheckNull(Ability);
	//CheckFalse(bReady);
	Ability->Behavior();
}

bool ACSkill::GetbStop()
{
	return Ability->GetbStop();
}

void ACSkill::Play(UAnimMontage * InMontage, float InPlayRatio, FName StartSectionName)
{
	ACharacter* character = Cast<ACharacter>(GetOwner());

	CheckNull(character);

	character->PlayAnimMontage(InMontage, InPlayRatio, StartSectionName);
}

void ACSkill::GetShapeComponent(TArray<class UShapeComponent*>& OutComponents)
{
	TArray<UActorComponent *> components = GetComponentsByClass(UShapeComponent::StaticClass());

	for (UActorComponent* component : components)
	{
		UShapeComponent* temp = Cast<UShapeComponent>(component);

		if (!!temp)
			OutComponents.Add(temp);
	}
}

void ACSkill::OnComponentBeginOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{

	CheckTrue(OtherActor == OwnerCharacter);
	CheckTrue(OtherActor == this);
	CheckTrue(Cast<ACNpc>(OtherActor) != NULL);
	ACharacter* character = Cast<ACharacter>(OtherActor);
	CheckNull(character);

	UCStateComponent* state = CHelpers::GetComponent<UCStateComponent>(character);
	CheckNull(state);
	CheckTrue(state->IsDead());
	CheckTrue(state->IsNone());

	float power = Ability->GetPower();


	FDamageEvent e;

	if (State->IsAttack())
	{
		FDamageEvent e;
		//210621 remake
		if (Ability != NULL)
		{
			if (Ability->ExistHitActor(Cast<ACharacter>(OtherActor)) == false)
			{
				const TArray<FMontageData>* data = Ability->GetMontagesDatas();
				/*if (data.HitStop > 1e-6f)
				{
					UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1e-3f);
					UKismetSystemLibrary::K2_SetTimer(this, "OnRestoreTimer", data.HitStop* 1e-3f, false);
				}*/
				power = Ability->GetPower(Ability->GetCount());
				if (data->GetData()[Ability->GetCount()].HitEffect != NULL)
				{
					UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), data->GetData()[Ability->GetCount()].HitEffect, OtherActor->GetActorLocation());
				}
				if (data->GetData()[Ability->GetCount()].HitSound != NULL)
					UGameplayStatics::SpawnSoundAttached(data->GetData()[Ability->GetCount()].HitSound, OtherActor->GetRootComponent());

				character->TakeDamage(power, e, OwnerCharacter->GetController(), this);
			}

			return;
		}
		character->TakeDamage(power, e, OwnerCharacter->GetController(), this);
	}
	if (State->IsSkill())
	{
		//210621 remake
		if (Ability!=NULL)
		{
			if (Ability->ExistHitActor(Cast<ACharacter>(OtherActor)) == false)
			{
				const FMontageData* data = Ability->GetMontageData();

				if (data->HitEffect != NULL)
				{
					UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), data->HitEffect, OtherActor->GetActorLocation());
				}
				if (data->HitSound != NULL)
					UGameplayStatics::SpawnSoundAttached(data->HitSound, OtherActor->GetRootComponent());

				character->TakeDamage(power, e, OwnerCharacter->GetController(), this);
			}

			return;
		}
		character->TakeDamage(power, e, OwnerCharacter->GetController(), this);
	}
}

void ACSkill::OnRestoreTimer()
{
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1);
}

void ACSkill::SkillUse()
{
	SkillResetTime = SkillCoolTime;
	SetbReady(false);
}

void ACSkill::SkillPreview()
{
	//decal·Î ¹üÀ§

	Ability->SkillPreview();
}

void ACSkill::Cancel()
{
	Ability->Cancel();
}

SkillInputType ACSkill::GetSkillInputType()
{
	return Ability->GetSkillInputType();
}

ACharacter * ACSkill::GetEnemy()
{
	return SkillComp->GetEnemy();
}

TArray<class UShapeComponent*> ACSkill::GetClassColliders()
{
	return ClassColliders;
}

void ACSkill::SetClassColliders(TArray<class UShapeComponent*> InComponents)
{
	ClassColliders = InComponents;
	for (UShapeComponent* component : ClassColliders)
	{
		component->OnComponentBeginOverlap.AddDynamic(this, &ACSkill::OnComponentBeginOverlap);
		component->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

//void ACSkill::SetMontageType(EMontageType InMontageType)
//{
//	MontageType = InMontageType; 
//	/*const UEnum* enumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EMontageType"), true);
//	if (!enumPtr)
//	{
//		CLog::Log(FString("Invalid"));
//	}
//	CLog::Log(enumPtr->GetEnumName((int32)InMontageType));*/
//	Ability->SetMontagesData(Montages->GetSkillData(MontageType));
//	CLog::Log("ACSkill::SetMontageType");
//	CLog::Log(Montages->GetSkillData(MontageType)->PlayRatio);
//}

void ACSkill::AttachToOwnerMesh(FName InSocket)
{
	CHelpers::AttachToComponent(this, OwnerCharacter->GetMesh(), InSocket);
}

void ACSkill::AttachComponentToOwnerMesh(USceneComponent * InComponent, FName InSocket)
{
	InComponent->AttachToComponent(OwnerCharacter->GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), InSocket);
}
