#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Parkour", meta = (ClampMin = "0", ClampMax = "1"))
	float SlidingFriction = 0.8f;

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

protected:

	/**
	 * Update all parkour moves.
	 *
	 * @param DeltaTime Time slice for the movement.
	 */
	virtual void UpdateParkourMovement(float DeltaTime);

	virtual void UpdateCrouching(float DeltaTime);
	virtual void UpdateSliding(float DeltaTime);

private:

	/** Whether the player is currently sliding */
	bool bIsSliding;

	/** How fast the player is currently sliding */
	float SlidingMovementSpeed;

	bool bIsParkourActionUpActive;
	bool bIsParkourActionDownActive;
};
