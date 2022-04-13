#pragma once
#include "CoreMinimal.h"
#include "GenGraph.h"
#include "Definitions.h"

class MAZE_API SolveMaze {
public:
	static TArray<Node> Solve(TArray<Node> MazeGraph, int** Maze, int Start, int Finish);
};
