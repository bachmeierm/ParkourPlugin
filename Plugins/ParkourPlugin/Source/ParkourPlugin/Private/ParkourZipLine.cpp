#include "ParkourZipLine.h"

AParkourZipLine::AParkourZipLine(const FObjectInitializer& ObjectInitiaizer) : Super(ObjectInitiaizer)
{
	SceneComponent = CreateDefaultSubobject<USceneComponent>(FName("SceneComponent"));
	SplineComponent = CreateDefaultSubobject<USplineComponent>(FName("SplineComponent"));
	SplineMeshComponent = CreateDefaultSubobject<USplineMeshComponent>(FName("SplineMeshComponent"));

	RootComponent = SceneComponent;

	FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, false);

	SplineComponent->AttachToComponent(SceneComponent, AttachmentRules);
	SplineMeshComponent->AttachToComponent(SceneComponent, AttachmentRules);

	SplineMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void AParkourZipLine::OnConstruction(const FTransform& Transform)
{
	FVector StartLocation;
	FVector EndLocation;
	FVector StartTangent;
	FVector EndTangent;
	SplineComponent->GetLocationAndTangentAtSplinePoint(0, StartLocation, StartTangent, ESplineCoordinateSpace::World);
	SplineComponent->GetLocationAndTangentAtSplinePoint(1, EndLocation, EndTangent, ESplineCoordinateSpace::World);
	SplineMeshComponent->SetStartAndEnd(StartLocation, StartTangent, EndLocation, EndTangent);
}
