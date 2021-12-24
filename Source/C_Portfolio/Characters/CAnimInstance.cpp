#include "CAnimInstance.h"
#include "Global.h"
#include "CPlayer.h"
#include "GameFramework/Character.h"

void UCAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	ACharacter* character = Cast<ACharacter>(TryGetPawnOwner());
	CheckNull(character);

	UCStateComponent* state = CHelpers::GetComponent<UCStateComponent>(character);
	CheckNull(state);
	state->OnStateTypeChanged.AddDynamic(this, &UCAnimInstance::StateTypeChanged);

	UCClassComponent* cclass = CHelpers::GetComponent<UCClassComponent>(character);
	CheckNull(cclass);
	cclass->OnClassTypeChanged.AddDynamic(this, &UCAnimInstance::ClassTypeChanged);

}

void UCAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	ACharacter* character = Cast<ACharacter>(TryGetPawnOwner());
	CheckNull(character);

	Speed = character->GetVelocity().Size2D();
	Direction = CalculateDirection(character->GetVelocity(), character->GetControlRotation());

}

void UCAnimInstance::ClassTypeChanged(EClassType InPrevType, EClassType InNewType)
{
	ClassType = InNewType;
}

void UCAnimInstance::StateTypeChanged(EStateType InPrevType, EStateType InNewType)
{
	StateType = InNewType;
}
