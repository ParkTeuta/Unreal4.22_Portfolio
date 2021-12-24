#include "CCameraShake.h"
#include "Global.h"

UCCameraShake::UCCameraShake()
{
	OscillationDuration = 0.2f;

	LocOscillation.Y.Amplitude = 100.0f;
	LocOscillation.Y.Frequency = 100.0f;

	LocOscillation.Z.Amplitude = 25.0f;
	LocOscillation.Z.Frequency = 50.0f;
}