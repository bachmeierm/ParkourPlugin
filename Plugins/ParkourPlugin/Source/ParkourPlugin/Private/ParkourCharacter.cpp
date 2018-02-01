#include "ParkourCharacter.h"

AParkourCharacter::AParkourCharacter(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer.
	SetDefaultSubobjectClass<UParkourMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	PrimaryActorTick.bCanEverTick = true;
}

UParkourMovementComponent* AParkourCharacter::GetParkourMovementComponent()
{
	return Cast<UParkourMovementComponent>(GetMovementComponent());
}

AParkourCameraManager* AParkourCharacter::GetParkourCameraManager()
{
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	return Cast<AParkourCameraManager>(PlayerController->PlayerCameraManager);
}
