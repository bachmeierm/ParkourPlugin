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

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

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

protected:

	/**
	 * Update all parkour moves.
	 *
	 * @param DeltaTime Time slice for the movement.
	 */
	virtual void UpdateParkourMovement(float DeltaTime);

	virtual void UpdateCrouching(float DeltaTime);

private:

	bool bIsParkourActionUpActive;
	bool bIsParkourActionDownActive;
};
