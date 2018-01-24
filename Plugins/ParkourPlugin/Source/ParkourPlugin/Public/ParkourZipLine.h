#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SceneComponent.h"
#include "Components/SplineComponent.h"
#include "Components/SplineMeshComponent.h"
#include "ParkourZipLine.generated.h"

UCLASS()
class PARKOURPLUGIN_API AParkourZipLine : public AActor
{
	GENERATED_BODY()
	
public:

	AParkourZipLine(const FObjectInitializer& ObjectInitiaizer);

	// Begin AActor interface
	virtual void OnConstruction(const FTransform& Transform) override;
	// End AActor interface

	UPROPERTY()
	USceneComponent* SceneComponent;

	/** Defines the shape of the zip line. */
	UPROPERTY(EditAnywhere)
	USplineComponent* SplineComponent;

	/** Visual representation mesh of the zip line. */
	UPROPERTY(EditAnywhere)
	USplineMeshComponent* SplineMeshComponent;

	/** Distance from the end of the zip line, where the character drops automatically. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ZipLine", meta = (ClampMin = "100"))
	float ZipLineAutoDropOffset = 100;
};
