#include "MazeSegment.h"

// Sets default values
AMazeSegment::AMazeSegment() {
	PrimaryActorTick.bCanEverTick = true;

	// underglow visual mesh
	PlaneMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GlowMesh"));
	PlaneMesh->SetupAttachment(RootComponent);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> PlaneVisualAsset(TEXT("/Game/MyContent/GlowMeshBlue.GlowMeshBlue"));

	// set up for runtime editable colors
	if (PlaneVisualAsset.Succeeded()) {
		PlaneMesh->SetStaticMesh(PlaneVisualAsset.Object);
		PlaneMesh->SetRelativeLocation(FVector(0, 0, 145));
		PlaneMesh->SetWorldScale3D(FVector(1, 1, 1));
		PlaneMesh->ComponentTags.Add(FName("Plane"));

		auto Material = PlaneMesh->GetMaterial(0);
		DynamicMaterial = UMaterialInstanceDynamic::Create(Material, NULL);
		PlaneMesh->SetMaterial(0, DynamicMaterial);
	}

	// floor/wall visual mesh
	VisualMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VisualMesh"));
	VisualMesh->SetupAttachment(PlaneMesh);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeVisualAsset(TEXT("/Game/MyContent/MapSegment.MapSegment"));

	if (CubeVisualAsset.Succeeded()) {
		VisualMesh->SetStaticMesh(CubeVisualAsset.Object);
		VisualMesh->SetRelativeLocation(FVector(0, 0, -145));
		VisualMesh->SetWorldScale3D(FVector(0.45,0.45,0.5));
	}

	// collider for character overlap set up
	BoxCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collider"));
	BoxCollider->SetupAttachment(VisualMesh);
	BoxCollider->SetBoxExtent(FVector(100,100,310));
	BoxCollider->SetCollisionProfileName("Trigger");
	BoxCollider->OnComponentBeginOverlap.AddDynamic(this, &AMazeSegment::OnSegmentBeginOverlap);
	BoxCollider->OnComponentEndOverlap.AddDynamic(this, &AMazeSegment::OnSegmentEndOverlap);
	
}

// change overlapped cell color so it is more obvious in overhead view
void AMazeSegment::OnSegmentBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	TArray<UStaticMeshComponent*> SegmentComponents;
	this->GetComponents(SegmentComponents);

	if (SegmentComponents[1]->GetComponentLocation().Z <= 0) {
		SegmentComponents[0]->SetVectorParameterValueOnMaterials(FName("Color"), FVector(1,1,1));
		SegmentComponents[1]->SetVectorParameterValueOnMaterials(FName("Color"), FVector(1, 1, 1));
	}
}
void AMazeSegment::OnSegmentEndOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {
	TArray<UStaticMeshComponent*> SegmentComponents;
	this->GetComponents(SegmentComponents);

	if (SegmentComponents[1]->GetComponentLocation().Z <= 0) {
		SegmentComponents[0]->SetVectorParameterValueOnMaterials(FName("Color"), FVector(0, 0.44, 0.5));
		SegmentComponents[1]->SetVectorParameterValueOnMaterials(FName("Color"), FVector(0.25,0.25,0.25));
	}
}