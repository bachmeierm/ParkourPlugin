#pragma once

#include "CoreMinimal.h"
#include "Camera/PlayerCameraManager.h"
#include "ParkourCameraManager.generated.h"

USTRUCT(BlueprintType)
struct FParkourCameraLimits
{
	GENERATED_USTRUCT_BODY()

public:

	FParkourCameraLimits()
	{
		PitchMin = -80;
		PitchMax = 80;
		YawMin = 0;
		YawMax = 360;
	}

	UPROPERTY(EditAnywhere)
	float PitchMin;

	UPROPERTY(EditAnywhere)
	float PitchMax;

	UPROPERTY(EditAnywhere)
	float YawMin;

	UPROPERTY(EditAnywhere)
	float YawMax;
};

UCLASS()
class PARKOURPLUGIN_API AParkourCameraManager : public APlayerCameraManager
{
	GENERATED_BODY()

public:

	/**
	 * Constrain the camera rotation by the given limits.
	 */
	void SetCameraLmits(FParkourCameraLimits Limits);

	/**
	 * Reset camera limits to the default value.
	 */
	void ResetCameraLimits();

protected:

	// Begin PlayerCameraManager interface
	virtual void LimitViewPitch(FRotator& ViewRotation, float InViewPitchMin, float InViewPitchMax) override;
	virtual void LimitViewYaw(FRotator& ViewRotation, float InViewYawMin, float InViewYawMax) override;
	// End PlayerCameraManager interface

	bool bUseCameraLimits;
	FParkourCameraLimits CameraLimits;
};
