#include "CMoveInteraction.h"
#include "Global.h"
#include "Components/SphereComponent.h"
#include "Components/SplineComponent.h"

#include "Components/CMoveInteractionComponent.h"

#include "Animation/AnimMontage.h"
#include "Characters/CPlayer.h"
#include "Components/DecalComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
ACMoveInteraction::ACMoveInteraction()
{
	PrimaryActorTick.bCanEverTick = true;

	Spline = CreateDefaultSubobject<USplineComponent>("Sphline");

	InteractionArea1 = CreateDefaultSubobject<USphereComponent>("InteractionArea1");
	InteractionArea1->SetSphereRadius(130.0f);
	InteractionArea1->SetupAttachment(Spline);

	InteractionArea1->OnComponentBeginOverlap.AddDynamic(this, &ACMoveInteraction::Area1BeginOverlap);
	InteractionArea1->OnComponentEndOverlap.AddDynamic(this, &ACMoveInteraction::AreaEndOverlap);

	InteractionArea2 = CreateDefaultSubobject<USphereComponent>("InteractionArea2");
	InteractionArea2->SetSphereRadius(130.0f);
	InteractionArea2->SetupAttachment(Spline);

	InteractionArea2->OnComponentBeginOverlap.AddDynamic(this, &ACMoveInteraction::Area2BeginOverlap);
	InteractionArea2->OnComponentEndOverlap.AddDynamic(this, &ACMoveInteraction::AreaEndOverlap);

	// Create a decal in the world to show the cursor's location
	AreaDecal1 = CreateDefaultSubobject<UDecalComponent>("AreaDecal1");
	AreaDecal1->SetupAttachment(RootComponent);
	UMaterial* DecalMaterialAsset;
	CHelpers::GetAssetDynamic<UMaterial>(&DecalMaterialAsset, "Material'/Game/Materials/Decal/M_Decal_Interaction.M_Decal_Interaction'");
	if (!!DecalMaterialAsset)
	{
		AreaDecal1->SetDecalMaterial(DecalMaterialAsset);
	}
	AreaDecal1->DecalSize = FVector(20.0f, 40.0f, 40.0f);
	AreaDecal1->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f).Quaternion());

	// Create a decal in the world to show the cursor's location
	AreaDecal2 = CreateDefaultSubobject<UDecalComponent>("AreaDecal2");
	AreaDecal2->SetupAttachment(RootComponent);
	if (!!DecalMaterialAsset)
	{
		AreaDecal2->SetDecalMaterial(DecalMaterialAsset);
	}
	AreaDecal2->DecalSize = FVector(20.0f, 40.0f, 40.0f);
	AreaDecal2->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f).Quaternion());




	bReverse = true;
}

void ACMoveInteraction::BeginPlay()
{
	Super::BeginPlay();

	FVector location1 = Spline->GetLocationAtSplinePoint(0, ESplineCoordinateSpace::World);
	FVector location2 = Spline->GetLocationAtSplinePoint(1, ESplineCoordinateSpace::World);
	InteractionArea1->SetWorldLocation(location1);
	InteractionArea2->SetWorldLocation(location2);
	location1.Z -= 23;
	location2.Z -= 23;
	AreaDecal1->SetWorldLocation(location1);
	AreaDecal2->SetWorldLocation(location2);
	//Spline->GetLocationAtSplinePoint()

}

void ACMoveInteraction::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void ACMoveInteraction::Area1BeginOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	CheckTrue(OtherActor == this);

	ACPlayer* player = Cast<ACPlayer>(OtherActor);
	CheckNull(player);

	StartIndex = 0;
	EndIndex = 1;

	UCMoveInteractionComponent* comp = CHelpers::GetComponent<UCMoveInteractionComponent>(player);
	CheckNull(comp);

	CheckTrue(comp->GetbMove());
	comp->SetMoveInteraction(this);
	
}

void ACMoveInteraction::Area2BeginOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	CheckFalse(bReverse);
	CheckTrue(OtherActor == this);

	ACPlayer* player = Cast<ACPlayer>(OtherActor);
	CheckNull(player);

	StartIndex = 1;
	EndIndex = 0;

	UCMoveInteractionComponent* comp = CHelpers::GetComponent<UCMoveInteractionComponent>(player);
	CheckNull(comp);

	CheckTrue(comp->GetbMove());
	comp->SetMoveInteraction(this);
}

void ACMoveInteraction::AreaEndOverlap(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	CheckTrue(OtherActor == this);

	ACPlayer* player = Cast<ACPlayer>(OtherActor);
	CheckNull(player);

	UCMoveInteractionComponent* comp = CHelpers::GetComponent<UCMoveInteractionComponent>(player);
	CheckNull(comp);

	CheckTrue(comp->GetbMove());

	CheckNull(comp->GetMoveInteraction());
	CheckFalse(comp->GetMoveInteraction() == this);

	comp->SetMoveInteraction(NULL);
}

