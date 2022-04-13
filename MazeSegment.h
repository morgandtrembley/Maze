#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Definitions.h"
#include "Components/BoxComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Materials/MaterialInterface.h"
#include "MazeSegment.generated.h"

UCLASS()
class MAZE_API AMazeSegment : public AActor {
	GENERATED_BODY()
	
public:	
	AMazeSegment();

	UFUNCTION()
	void OnSegmentBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnSegmentEndOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY(VisibleAnywhere, Category = "Switch Components")
	UStaticMeshComponent* VisualMesh;
	UStaticMeshComponent* PlaneMesh;
	UBoxComponent* BoxCollider;

	UMaterialInstanceDynamic* DynamicMaterial;
};
