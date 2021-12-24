#include "CJumpDownObject.h"
#include "Global.h"
#include "Components/SphereComponent.h"

ACJumpDownObject::ACJumpDownObject()
{
	SphereCollision = CreateDefaultSubobject<USphereComponent>("SphereCollision");
}
