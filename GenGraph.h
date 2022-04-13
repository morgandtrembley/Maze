#pragma once
#include "CoreMinimal.h"
#include "Definitions.h"

class MAZE_API GenGraph {
public:
	static TArray<Node> Generate(int** Maze);
};
