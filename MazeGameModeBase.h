#pragma once
#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Definitions.h"
#include "MazeSegment.h"
#include "MazeGen.h"
#include "GenGraph.h"
#include "SolveMaze.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "MazeGameModeBase.generated.h"

UCLASS()
class MAZE_API AMazeGameModeBase : public AGameModeBase {
	GENERATED_BODY()

	int WalkwaySize = 5;
	int** GeneratedMaze;
	bool SolutionShown = false;

	AMazeSegment* NewMazeCell;
	TArray<UStaticMeshComponent*> TempMeshComponents;
	UStaticMeshComponent* TempMesh;
	UMaterialInstanceDynamic* TempMaterial;

	TArray<AMazeSegment*> Maze;

	TArray<Node> MazeGraph;
	TArray<Node> Solution;

protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "Maze Manipulation")
	void ConstructMaze();

	UFUNCTION(BlueprintCallable, Category = "Maze Manipulation")
	void ResetMaze();

	UFUNCTION(BlueprintCallable, Category = "Maze Manipulation")
	void PrepMaze();

	UFUNCTION(BlueprintCallable, Category = "Maze Manipulation")
	void ShowSolution();

	void InitPlayArea();
	void SetWallColor();
	
public:
	AMazeGameModeBase();
};
