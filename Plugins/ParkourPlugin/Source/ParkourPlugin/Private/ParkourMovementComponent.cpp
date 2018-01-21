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

void UParkourMovementComponent::SetParkourActionUp(bool IsActive)
{
	bIsParkourActionUpActive = IsActive;
}

void UParkourMovementComponent::SetParkourActionDown(bool IsActive)
{
	bIsParkourActionDownActive = IsActive;
}

void UParkourMovementComponent::UpdateParkourMovement(float DeltaTime)
{
	UpdateCrouching(DeltaTime);
}

void UParkourMovementComponent::UpdateCrouching(float DeltaTime)
{
	bWantsToCrouch = bIsParkourActionDownActive && MovementMode == MOVE_Walking;
}
