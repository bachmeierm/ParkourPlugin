#pragma once

#include "CoreMinimal.h"
#include "ParkourZipLine.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "ParkourMovementComponent.generated.h"

UENUM(BlueprintType)
enum class EParkourMovementMode : uint8
{
	ZipLine = 0,
};

/**
 * 
 */
UCLASS()
class PARKOURPLUGIN_API UParkourMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()
	
public:

	UParkourMovementComponent(const FObjectInitializer& ObjectInitializer);

	// Begin UActorComponent interface
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	// End UActorComponent interface

	// Being UMovementComponent interface
	virtual float GetMaxSpeed() const override;
	// End UMovementComponent interface

	/**
	 * Controls how fast the player decelerates while sliding.
	 * Lower values let the player slide longer (0 allows the player to slide indefinitely long, until he stops crouching),
	 * where as higher values shorten the sliding period.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Parkour | Sliding", meta = (ClampMin = "0", ClampMax = "1"))
	float SlidingFriction = 0.8f;

	/**
	 * Minimum movement angle for which the player can coil jump.
	 * If the angle drops below this value the coil jump is terminated.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Parkour | CoilJump", meta = (ClampMin = "-80", ClampMax = "0" ))
	float CoilJumpAngleMin = -20;

	/**
	 * Maximum movement angle for which the player can coil jump.
	 * If the angle rises above this value the coil jump is terminated.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Parkour | CoilJump", meta = (ClampMin = "0", ClampMax = "80"))
	float CoilJumpAngleMax = 45;

	/**
	 * Collision half height of the character capsule while coil jumping.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Parkour | CoilJump")
	float CoilJumpCapsuleHalfHeight = 44;

	/**
	 * The center of the zip line trigger sphere relative to the character location.
	 * When the sphere overlaps the zip line, the character is automatically attached to it.
	 * In most cases this should be slightly above the character, where his hands
	 * would be if he put them up.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Parkour")
	FVector ZipLineTriggerOffset = FVector(0, 0, 100);

	/**
	 * The radius of the zip line trigger sphere. Very small values make it hard to jump onto the zip line,
	 * whereas large values could accidentally trigger zip line attachments.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Parkour", meta = (ClampMin = "10", ClampMax = "500"))
	float ZipLineTriggerRadius = 100;

	/** Speed the character gains per second, while sliding down a zip line. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Parkour", meta = (ClampMin = "0"))
	float ZipLineAcceleration = 400;

	/** The maximum speed possible while sliding down a zip line. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Parkour", meta = (ClampMin = "0"))
	float ZipLineMaxSpeed = 15000;

	/** The speed at which the character rotation is interpolated to the sliding angle. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Parkour", meta = (ClampMin = "0"))
	float ZipLineInterpolationSpeed = 100;

	/** Minimum required height difference between jump start location and boost platform to trigger a spring board movement. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Parkour", meta = (ClampMin = "0"))
	float SpringBoardHeightDeltaMin = 50;

	/** Maximum required height difference between jump start location and boost platform to trigger a spring board movement. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Parkour", meta = (ClampMin = "0"))
	float SpringBoardHeightDeltaMax = 150;

	/** The angle (forward being 0 and upward being 90) at which the character is boosted when spring boarding. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Parkour", meta = (ClampMin = "0", ClampMax = "90" ))
	float SpringBoardBoostAngle = 45;

	/** How strong the character is boosted when spring boarding. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Parkour", meta = (ClampMin = "0"))
	float SpringBoardBoostStrength = 1000;

	/**
	 * Time in seconds, the character is not able to attach to a zip line after dropping
	 * from a previous zip line move.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Parkour", meta = (ClampMin = "0"))
	float ZipLineTimeout = 1;

	/**
	 * Trigger upward movements like jumping and climbing.
	 *
	 * @param IsActive Whether the upwards action should be active.
	 */
	UFUNCTION(BlueprintCallable, Category = "Parkour")
	void SetParkourActionUp(bool IsActive);

	/**
	 * Trigger downward movements like crouching, sliding and releasing from ledges.
	 *
	 * @param IsActive Whether the downwards action should be active.
	 */
	UFUNCTION(BlueprintCallable, Category = "Parkour")
	void SetParkourActionDown(bool IsActive);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Parkour")
	bool IsSliding();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Parkour")
	bool IsCoilJumping();

protected:

	// Begin UCharacterMovementComponent interface
	virtual void OnMovementModeChanged(EMovementMode PreviousMovementMode, uint8 PreviousCustomMode) override;
	// End UCharacterMovementComponent interface

	bool bIsSliding;
	float SlidingMovementSpeed;

	bool bIsCoilJumping;

	AParkourZipLine* ZipLine = nullptr;
	float ZipLineOffset;
	float ZipLineSpeed;
	float ZipLineDropTime;

	bool bDidSpringBoard;

	FVector FallingStartLocation;

	bool bIsParkourActionUpActive;
	bool bIsParkourActionDownActive;

	virtual void UpdateParkourMovement(float DeltaTime);

	virtual void UpdateCrouching(float DeltaTime);
	virtual void UpdateSliding(float DeltaTime);
	virtual void UpdateJumping(float DeltaTime);
	virtual void UpdateCoilJumping(float DeltaTime);
	virtual void UpdateZipLine(float DeltaTime);
	virtual void UpdateSpringBoard(float DeltaTime);

	/**
	 * Set the height of the owning character capsule.
	 *
	 * @param HalfHeight Half height from capsule center to end of top or bottom hemisphere. 
	 */
	void SetCapsuleHalfHeight(float HalfHeight);

	/**
	 * Reset the height of the owning character capsule to the default value.
	 */
	void ResetCapsuleHalfHeight();
};
