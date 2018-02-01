#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ParkourMovementComponent.h"
#include "ParkourCameraManager.h"
#include "ParkourCharacter.generated.h"

UCLASS()
class PARKOURPLUGIN_API AParkourCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	
	AParkourCharacter(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	UParkourMovementComponent* GetParkourMovementComponent();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	AParkourCameraManager* GetParkourCameraManager();
};
