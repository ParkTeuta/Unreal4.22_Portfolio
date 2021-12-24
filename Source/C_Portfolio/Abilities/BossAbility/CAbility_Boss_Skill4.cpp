#include "CAbility_Boss_Skill4.h"
#include "Global.h"
#include "GameFramework/Character.h"

#include "Characters/CPlayer.h"
#include "Characters/CEnemy_Boss.h"
#include "Characters/CNpc.h"

#include "Components/CapsuleComponent.h"
#include "Components/CSkillComponent.h"
#include "Components/CStateComponent.h"
#include "Components/ShapeComponent.h"
#include "Animation/AnimMontage.h"

#include "Weapons/CSkill.h"
#include "Weapons/BossObject/CBossSkill4_ThrowItem.h"

#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"

ACAbility_Boss_Skill4::ACAbility_Boss_Skill4()
{
}

void ACAbility_Boss_Skill4::BeginPlay()
{
	Super::BeginPlay();

	TArray<UShapeComponent*> components;
	OwnerSkill->GetShapeComponent(components);
	InputType = SkillInputType::Enemy;

	for (UShapeComponent* component : components)
	{
		component->OnComponentBeginOverlap.AddDynamic(this, &ACAbility_Boss_Skill4::OnComponentBeginOverlap);
		component->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	//OwnerSkill->SetRange(200.0f);
}

void ACAbility_Boss_Skill4::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bColorChange == true)
	{
		ColorValue -= DeltaTime / 2;
		ColorValue = FMath::Clamp(ColorValue, 0.0f, 1.0f);
		OwnerBoss->SetBladeColor(FLinearColor(1, ColorValue, ColorValue));

		if (ColorValue == 0.0f)
		{
			OwnerBoss->StopAnimMontage();
			if (MontageDatas->Num() > 1)
			{
				const FMontageData& data = MontageDatas->GetData()[1];
				OwnerCharacter->PlayAnimMontage(data.AnimMontage, data.PlayRatio, data.StartSection);
				bColorChange = false;
				if (Particle != NULL)
					Particle->DestroyComponent();
			}
			else
			{
				End_Behavior();
				bColorChange = false;
				if (Particle != NULL)
					Particle->DestroyComponent();
			}
		}
	}
}

void ACAbility_Boss_Skill4::Behavior()
{
	OwnerBoss = Cast<ACEnemy_Boss>(OwnerCharacter);
	CheckNull(OwnerBoss);

	Super::Behavior();

	CheckFalse(State->IsIdle());
	CheckFalse(OwnerSkill->GetbReady());
	State->SetSkillMode();

	CheckNull(MontageDatas);
	OwnerCharacter->GetCharacterMovement()->StopMovementImmediately();

	const FMontageData& data = MontageDatas->GetData()[0];
	OwnerCharacter->PlayAnimMontage(data.AnimMontage, data.PlayRatio, data.StartSection);

	ColorValue = 1.0f;
	bColorChange = true;

	OwnerSkill->SetbReady(false);
	OwnerSkill->SetSkillCool();

	if (GaugeParticle != NULL)
		Particle = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), GaugeParticle, OwnerBoss->GetTransform());

	if (RangeClass != NULL)
	{
		FActorSpawnParameters params;
		params.Owner = this;

		RangeObject = GetWorld()->SpawnActor<AActor>(RangeClass, OwnerBoss->GetTransform(), params);
	}

}

void ACAbility_Boss_Skill4::Begin_Behavior() //클릭하면
{

	FActorSpawnParameters params;
	params.Owner = this;

	if (ThrowItemClass != NULL && RangeObject != NULL)
	{
		for (FTransform transform : GetRangeTransforms())
		{
			ACBossSkill4_ThrowItem* throwItem = GetWorld()->SpawnActor<ACBossSkill4_ThrowItem>(ThrowItemClass, transform, params);
			throwItem->SetOwnerSkill(OwnerSkill);
		}
	}

	//Next Range
	OwnerBoss->SetBladeColor(FLinearColor(0, 0, 0));
	if (RangeObject != NULL)
	{
		RangeObject->Destroy();

		FActorSpawnParameters params;
		params.Owner = this;

		FTransform transform = OwnerBoss->GetTransform();
		FRotator rot = transform.GetRotation().Rotator();
		rot.Yaw += 30;
		transform.SetRotation(rot.Quaternion());

		RangeObject = GetWorld()->SpawnActor<AActor>(RangeClass, transform, params);
	}
}

void ACAbility_Boss_Skill4::Middle_Behavior()
{

	FActorSpawnParameters params;
	params.Owner = this;

	if (ThrowItemClass != NULL && RangeObject != NULL)
	{
		for (FTransform transform : GetRangeTransforms())
		{
			ACBossSkill4_ThrowItem* throwItem = GetWorld()->SpawnActor<ACBossSkill4_ThrowItem>(ThrowItemClass, transform, params);
			throwItem->SetOwnerSkill(OwnerSkill);
		}
	}

	if (RangeObject != NULL)
		RangeObject->Destroy();
}

void ACAbility_Boss_Skill4::End_Behavior()
{
	bStop = false;

	State->SetIdleMode();
	OwnerBoss->SetBladeColor(FLinearColor(1, 1, 1));
	ColorValue = 1.0f;
	//Object->Destroy();
	//CHelpers::SetCollisionPresetPawn(OwnerCharacter->GetCapsuleComponent());
}

void ACAbility_Boss_Skill4::OnColliders()
{
	TArray<UShapeComponent*> components;
	OwnerSkill->GetShapeComponent(components);

	for (UShapeComponent* component : components)
		component->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	//CLog::Log("OnColliders");
}

void ACAbility_Boss_Skill4::OffColliders()
{
	TArray<UShapeComponent*> components;
	OwnerSkill->GetShapeComponent(components);

	for (UShapeComponent* component : components)
		component->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	HitActors.Empty();
}

void ACAbility_Boss_Skill4::SkillPreview()
{

}

void ACAbility_Boss_Skill4::Cancel()
{
	CheckFalse(bStop);
	OffColliders();
	End_Behavior();
}

void ACAbility_Boss_Skill4::OnComponentBeginOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	CheckTrue(OtherActor == OwnerCharacter);
	CheckTrue(OtherActor == this);
	CheckTrue(Cast<ACNpc>(OtherActor) != NULL);
	ACharacter* character = Cast<ACharacter>(OtherActor);
	CheckNull(character);

	float power = GetPower();

	if (State->IsSkill())
	{
		if (ExistHitActor(Cast<ACharacter>(OtherActor)) == false)
		{
			const FMontageData* data = GetMontageData();
			if (data->HitEffect != NULL)
			{
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), data->HitEffect, OtherActor->GetActorLocation());
			}

			FDamageEvent e;
			character->TakeDamage(power, e, OwnerCharacter->GetController(), this);
		}
	}
}


//float ACAbility_Boss_Skill4::GetPower()
//{
//	/*const FMontageData& data = MontageDatas->GetData()[Count];
//
//	return data.Power;*/
//}