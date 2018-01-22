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

bool UParkourMovementComponent::IsCoilJumping()
{
	return bIsCoilJumping;
}

void UParkourMovementComponent::UpdateParkourMovement(float DeltaTime)
{
	UpdateCrouching(DeltaTime);
	UpdateSliding(DeltaTime);
	UpdateJumping(DeltaTime);
	UpdateCoilJumping(DeltaTime);
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

void UParkourMovementComponent::UpdateCoilJumping(float DeltaTime)
{
	bool CancelCoilJump = false;
	float AngleOffset = 0;

	if (MovementMode != MOVE_Falling || !bIsParkourActionDownActive)
	{
		CancelCoilJump = true;
	}

	if (!CancelCoilJump)
	{
		FVector VelocityDirection = Velocity;
		VelocityDirection.Normalize();
		float DotProduct = FVector::DotProduct(VelocityDirection, FVector::UpVector);
		float AngleRadians = FMath::Acos(DotProduct);
		float AngleDegree = FMath::RadiansToDegrees(AngleRadians);
		AngleOffset = 90.0f - AngleDegree;
	}

	if (CancelCoilJump || AngleOffset < CoilJumpAngleMin || AngleOffset > CoilJumpAngleMax)
	{
		if (bIsCoilJumping)
		{
			ResetCapsuleHalfHeight();
			bIsCoilJumping = false;
		}

		return;
	}

	bIsCoilJumping = true;
	SetCapsuleHalfHeight(CoilJumpCapsuleHalfHeight);
}

void UParkourMovementComponent::SetCapsuleHalfHeight(float HalfHeight)
{
	ACharacter* Character = GetCharacterOwner();
	UCapsuleComponent* CapsuleComponent = Character->GetCapsuleComponent();
	CapsuleComponent->SetCapsuleHalfHeight(HalfHeight);
}

void UParkourMovementComponent::ResetCapsuleHalfHeight()
{
	UClass* CharacterClass = CharacterOwner->GetClass();
	ACharacter* DefaultCharacter = CharacterClass->GetDefaultObject<ACharacter>();
	UCapsuleComponent* DefaultCapsuleComponent = DefaultCharacter->GetCapsuleComponent();
	float DefaultCapsuleHalfHeight = DefaultCapsuleComponent->GetUnscaledCapsuleHalfHeight();
	SetCapsuleHalfHeight(DefaultCapsuleHalfHeight);
}
