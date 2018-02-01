#include "ParkourCameraManager.h"

void AParkourCameraManager::SetCameraLmits(FParkourCameraLimits Limits)
{
	bUseCameraLimits = true;
	CameraLimits = Limits;
}

void AParkourCameraManager::ResetCameraLimits()
{
	bUseCameraLimits = false;
}

void AParkourCameraManager::LimitViewPitch(FRotator& ViewRotation, float InViewPitchMin, float InViewPitchMax)
{
	if (bUseCameraLimits)
	{
		Super::LimitViewPitch(ViewRotation, CameraLimits.PitchMin, CameraLimits.PitchMax);
	}
	else
	{
		Super::LimitViewPitch(ViewRotation, InViewPitchMin, InViewPitchMax);
	}
}

void AParkourCameraManager::LimitViewYaw(FRotator& ViewRotation, float InViewYawMin, float InViewYawMax)
{
	APawn* Pawn = GetViewTargetPawn();

	if (bUseCameraLimits && Pawn)
	{
		FRotator PawnRotation = Pawn->GetActorRotation();
		float YawMin = CameraLimits.YawMin + PawnRotation.Yaw;
		float YawMax = CameraLimits.YawMax + PawnRotation.Yaw;

		Super::LimitViewYaw(ViewRotation, YawMin, YawMax);
	}
	else
	{
		Super::LimitViewYaw(ViewRotation, InViewYawMin, InViewYawMax);
	}
}
