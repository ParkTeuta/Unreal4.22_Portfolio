#include "CAbility_Boss_Skill2.h"
#include "Global.h"
#include "GameFramework/Character.h"
//#include "Characters/CPlayer.h"
#include "Characters/CEnemy_Boss.h"
#include "Characters/CNpc.h"

#include "Components/CSkillComponent.h"
#include "Components/CStateComponent.h"
#include "Components/ShapeComponent.h"
#include "Animation/AnimMontage.h"

#include "Weapons/CSkill.h"

ACAbility_Boss_Skill2::ACAbility_Boss_Skill2()
{
}

void ACAbility_Boss_Skill2::BeginPlay()
{
	Super::BeginPlay();

	TArray<UShapeComponent*> components;
	OwnerSkill->GetShapeComponent(components);
	InputType = SkillInputType::Enemy;

	for (UShapeComponent* component : components)
	{
		component->OnComponentBeginOverlap.AddDynamic(this, &ACAbility_Boss_Skill2::OnComponentBeginOverlap);
		component->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	//OwnerSkill->SetRange(200.0f);
}

void ACAbility_Boss_Skill2::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACAbility_Boss_Skill2::Behavior()
{
	OwnerBoss = Cast<ACEnemy_Boss>(OwnerCharacter);
	CheckNull(OwnerBoss);

	Super::Behavior();

	CheckFalse(State->IsIdle());
	CheckFalse(OwnerSkill->GetbReady());
	State->SetSkillMode();

	CheckNull(MontageDatas);
	OwnerCharacter->GetCharacterMovement()->StopMovementImmediately();

	FRotator rot = CHelpers::YawRotator(UKismetMathLibrary::FindLookAtRotation(OwnerCharacter->GetActorLocation(), OwnerBoss->GetAggro()->GetActorLocation()));
	OwnerCharacter->SetActorRotation(rot);

	OwnerCharacter->PlayAnimMontage(MontageData->AnimMontage, MontageData->PlayRatio, MontageData->StartSection);
	CHelpers::SetCollisionPresetIgnoreOnlyPawn(OwnerCharacter->GetCapsuleComponent());
}
void ACAbility_Boss_Skill2::Begin_Behavior() //클릭하면
{
	OwnerSkill->SetbReady(false);
	OwnerSkill->SetSkillCool();

	//여기서 미리 스폰 (그 범위 보여주는 용도로..)
	FActorSpawnParameters params;
	params.Owner = this;

	FTransform transform = OwnerCharacter->GetTransform();
	FVector location = transform.GetLocation();
	transform.SetLocation(location);

	/*MiddleObject = GetWorld()->SpawnActor<ACBossSkill1_Middle_Object>(MiddleObjectClass, transform, params);
	MiddleObject->SetOwnerSkill(OwnerSkill);*/
	OwnerBoss->SetBladeColor(FLinearColor(0, 0, 0));
}


void ACAbility_Boss_Skill2::End_Behavior()
{
	bStop = false;
	
	State->SetIdleMode();
	OwnerBoss->SetBladeColor(FLinearColor(1, 1, 1));
	CHelpers::SetCollisionPresetPawn(OwnerCharacter->GetCapsuleComponent());
}

void ACAbility_Boss_Skill2::OnColliders()
{
	TArray<UShapeComponent*> components;
	OwnerSkill->GetShapeComponent(components);

	for (UShapeComponent* component : components)
		component->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	//CLog::Log("OnColliders");
}

void ACAbility_Boss_Skill2::OffColliders()
{
	TArray<UShapeComponent*> components;
	OwnerSkill->GetShapeComponent(components);

	for (UShapeComponent* component : components)
		component->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	HitActors.Empty();
}

void ACAbility_Boss_Skill2::SkillPreview()
{

}

void ACAbility_Boss_Skill2::Cancel()
{
	CheckFalse(bStop);
	OffColliders();
	End_Behavior();
}

void ACAbility_Boss_Skill2::OnComponentBeginOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
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


//float ACAbility_Boss_Skill2::GetPower()
//{
//	/*const FMontageData& data = MontageDatas->GetData()[Count];
//
//	return data.Power;*/
//}