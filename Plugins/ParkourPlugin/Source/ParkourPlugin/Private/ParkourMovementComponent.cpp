#include "ParkourMovementComponent.h"

UParkourMovementComponent::UParkourMovementComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	NavAgentProps.bCanCrouch = true;
	bCanWalkOffLedgesWhenCrouching = true;
}

void UParkourMovementComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	UpdateParkourMovement(DeltaTime);
}

float UParkourMovementComponent::GetMaxSpeed() const
{
	if (MovementMode == MOVE_Walking && IsCrouching() && bIsSliding)
	{
		return SlidingMovementSpeed;
	}

	return Super::GetMaxSpeed();
}

void UParkourMovementComponent::SetParkourActionUp(bool IsActive)
{
	bIsParkourActionUpActive = IsActive;
}

void UParkourMovementComponent::SetParkourActionDown(bool IsActive)
{
	bIsParkourActionDownActive = IsActive;
}

bool UParkourMovementComponent::IsSliding()
{
	return bIsSliding;
}

void UParkourMovementComponent::UpdateParkourMovement(float DeltaTime)
{
	UpdateCrouching(DeltaTime);
	UpdateSliding(DeltaTime);
	UpdateJumping(DeltaTime);
}

void UParkourMovementComponent::UpdateCrouching(float DeltaTime)
{
	bWantsToCrouch = bIsParkourActionDownActive && MovementMode == MOVE_Walking;
}

void UParkourMovementComponent::UpdateSliding(float DeltaTime)
{
	float MovementSpeed = Velocity.Size();

	if (MovementMode != MOVE_Walking || !bIsParkourActionDownActive || MovementSpeed <= MaxWalkSpeedCrouched + 1)
	{
		bIsSliding = false;
		return;
	}

	if (!bIsSliding) SlidingMovementSpeed = MovementSpeed;

	SlidingMovementSpeed -= SlidingMovementSpeed * SlidingFriction * DeltaTime;
	SlidingMovementSpeed = FMath::Clamp(SlidingMovementSpeed, MaxWalkSpeedCrouched, MAX_flt);
	bIsSliding = true;
}

void UParkourMovementComponent::UpdateJumping(float DeltaTime)
{
	if (MovementMode == MOVE_Walking && bIsParkourActionUpActive && !IsCrouching())
	{
		DoJump(false);
	}
}
