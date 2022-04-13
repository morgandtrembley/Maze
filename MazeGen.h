#pragma once
#include "CoreMinimal.h"
#include "Definitions.h"

class MAZE_API MazeGen {
public:
	static int** GenerateMaze(int NumCellsX, int NumCellsY);

	static TArray<MazeCell> SetWalls(MazeCell** Maze, TArray<MazeCell> Cells, int Cols, int Rows);

	static int Contains(TArray<MazeCell> Cells, int CurrentCellID);

	static bool CheckForLoop(TArray<MazeCell> PossibleRoute);

	static void DropWalls(MazeCell** Maze, TArray<MazeCell> Cells, int NumCellsX, int NumCellsY);

	static TArray<MazeCell>  MoveNorth(MazeCell** Maze, TArray<MazeCell> PossibleRoute, int* CellLocation);
	static TArray<MazeCell>  MoveSouth(MazeCell** Maze, TArray<MazeCell> PossibleRoute, int* CellLocation);
	static TArray<MazeCell>  MoveWest(MazeCell** Maze, TArray<MazeCell> PossibleRoute, int* CellLocation);
	static TArray<MazeCell>  MoveEast(MazeCell** Maze, TArray<MazeCell> PossibleRoute, int* CellLocation);
};
