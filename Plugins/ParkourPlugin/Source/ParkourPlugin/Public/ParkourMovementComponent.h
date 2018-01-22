#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "ParkourMovementComponent.generated.h"

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

	bool bIsSliding;
	float SlidingMovementSpeed;

	bool bIsCoilJumping;

	bool bIsParkourActionUpActive;
	bool bIsParkourActionDownActive;

	virtual void UpdateParkourMovement(float DeltaTime);

	virtual void UpdateCrouching(float DeltaTime);
	virtual void UpdateSliding(float DeltaTime);
	virtual void UpdateJumping(float DeltaTime);
	virtual void UpdateCoilJumping(float DeltaTime);

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
