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

void UParkourMovementComponent::OnMovementModeChanged(EMovementMode PreviousMovementMode, uint8 PreviousCustomMode)
{
	Super::OnMovementModeChanged(PreviousMovementMode, PreviousCustomMode);

	if (PreviousMovementMode == MOVE_Walking && MovementMode == MOVE_Falling)
	{
		ACharacter* Character = GetCharacterOwner();
		UCapsuleComponent* CharacterCapsule = Character->GetCapsuleComponent();
		float CapsuleHalfHeight = CharacterCapsule->GetScaledCapsuleHalfHeight();
		FallingStartLocation = GetActorLocation() - FVector(0, 0, CapsuleHalfHeight);
	}
}

void UParkourMovementComponent::UpdateParkourMovement(float DeltaTime)
{
	UpdateCrouching(DeltaTime);
	UpdateSliding(DeltaTime);
	UpdateJumping(DeltaTime);
	UpdateCoilJumping(DeltaTime);
	UpdateZipLine(DeltaTime);
	UpdateSpringBoard(DeltaTime);
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

void UParkourMovementComponent::UpdateZipLine(float DeltaTime)
{
	UWorld* World = GetWorld();
	ACharacter* Character = GetCharacterOwner();

	if (MovementMode == MOVE_Custom && CustomMovementMode == (uint8)EParkourMovementMode::ZipLine)
	{
		float ZipLineLength = ZipLine->SplineComponent->GetSplineLength();

		float UpdatedSpeed = ZipLineSpeed + ZipLineAcceleration * DeltaTime;
		ZipLineSpeed = FMath::Clamp(UpdatedSpeed, 0.0f, ZipLineMaxSpeed);
		ZipLineOffset += 1 / ZipLineLength * ZipLineSpeed * DeltaTime;

		float RemainingDistance = ZipLineLength - ZipLineOffset * ZipLineLength;

		FVector Tangent = ZipLine->SplineComponent->GetTangentAtTime(ZipLineOffset, ESplineCoordinateSpace::World);
		Tangent.Normalize();

		if (RemainingDistance <= ZipLine->ZipLineAutoDropOffset || bIsParkourActionDownActive)
		{
			Velocity = Tangent * ZipLineSpeed;
			ZipLineDropTime = World->TimeSeconds;
			Character->bUseControllerRotationYaw = true;
			SetMovementMode(MOVE_Falling);
			return;
		}

		FVector TargetLocation = ZipLine->SplineComponent->GetLocationAtTime(ZipLineOffset, ESplineCoordinateSpace::World) - ZipLineTriggerOffset;
		FRotator TargetRotation = Tangent.Rotation();
		TargetRotation.Pitch = 0;
		TargetRotation.Roll = 0;

		FVector UpdatedLocation = FMath::VInterpConstantTo(Character->GetActorLocation(), TargetLocation, DeltaTime, ZipLineSpeed);
		FRotator UpdatedRotation = FMath::RInterpConstantTo(Character->GetActorRotation(), TargetRotation, DeltaTime, ZipLineInterpolationSpeed);

		Character->SetActorLocation(UpdatedLocation);
		Character->SetActorRotation(UpdatedRotation);
	}
	else if (MovementMode == MOVE_Falling && World->TimeSeconds - ZipLineDropTime > ZipLineTimeout)
	{
		FVector TriggerLocation = Character->GetActorLocation() + ZipLineTriggerOffset;

		FCollisionObjectQueryParams ObjectParams;
		FCollisionQueryParams Params;
		Params.bTraceComplex = true;

		TArray<FOverlapResult> Overlaps;
		if (World->OverlapMultiByObjectType(Overlaps, TriggerLocation, FQuat::Identity, ObjectParams, FCollisionShape::MakeSphere(ZipLineTriggerRadius), Params))
		{
			for (FOverlapResult Overlap : Overlaps)
			{
				AActor* Actor = Overlap.GetActor();
				if (Actor && Actor->GetClass()->IsChildOf(AParkourZipLine::StaticClass()))
				{
					ZipLine = Cast<AParkourZipLine>(Overlap.GetActor());
					ZipLineOffset = ZipLine->SplineComponent->FindInputKeyClosestToWorldLocation(TriggerLocation);
					ZipLineSpeed = FMath::Clamp(Velocity.Size(), 0.0f, ZipLineMaxSpeed);;

					Character->bUseControllerRotationYaw = false;

					SetMovementMode(MOVE_Custom, (uint8)EParkourMovementMode::ZipLine);
				}
			}
		}
	}
}

void UParkourMovementComponent::UpdateSpringBoard(float DeltaTime)
{
	if (MovementMode == MOVE_Falling)
	{
		if (!bIsParkourActionUpActive || bDidSpringBoard) return;

		UWorld* World = GetWorld();
		ACharacter* Character = GetCharacterOwner();

		FVector Start = Character->GetActorLocation();
		FVector End = Start - FVector(0, 0, SpringBoardHeightDeltaMax);
		FCollisionShape CollisionShape = GetPawnCapsuleCollisionShape(UCharacterMovementComponent::SHRINK_None);

		FHitResult Hit;
		if (World->SweepSingleByChannel(Hit, Start, End, FQuat::Identity, ECC_Visibility, CollisionShape))
		{
			float HeightDelta = Hit.ImpactPoint.Z - FallingStartLocation.Z;

			if (HeightDelta > SpringBoardHeightDeltaMin && HeightDelta < SpringBoardHeightDeltaMax)
			{
				FVector Direction = Character->GetActorForwardVector();
				FVector LeftVector = -Character->GetActorRightVector();
				Direction = Direction.RotateAngleAxis(SpringBoardBoostAngle, LeftVector);
				Velocity = Direction * SpringBoardBoostStrength;;
				bDidSpringBoard = true;
			}
		}
	}
	else
	{
		bDidSpringBoard = false;
	}
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
