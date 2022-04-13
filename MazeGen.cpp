#include "MazeGen.h"

// if Maze contains current cell return index of cell
int MazeGen::Contains(TArray<MazeCell> Cells, int CurrentCellID) {
	int i;
	for (i = 0; i < Cells.Num() - 1; i++) {
		if (Cells[i].ObjectID == CurrentCellID) {
			return i;
		}
	}
	return -1;
}

// if path contains a loop, return true
bool MazeGen::CheckForLoop(TArray<MazeCell> PossibleRoute) {
	bool loop = false;
	int i, CurrentCellID;
	CurrentCellID = PossibleRoute.Last().ObjectID;

	for (i = 0; i < PossibleRoute.Num() - 2; i++) {
		if (PossibleRoute[i].ObjectID == CurrentCellID) {
			loop = true;
		}
	}
	return loop;
}

/*** For All Move Functions: Move 1 step in the Function's given direction ***/

TArray<MazeCell> MazeGen::MoveNorth(MazeCell** Maze, TArray<MazeCell> PossibleRoute, int* CellLocation) {
	PossibleRoute.Add(Maze[CellLocation[ROW] + 1][CellLocation[COL]]);
	PossibleRoute.Add(Maze[CellLocation[ROW] + 2][CellLocation[COL]]);
	return PossibleRoute;
}
TArray<MazeCell> MazeGen::MoveSouth(MazeCell** Maze, TArray<MazeCell> PossibleRoute, int* CellLocation) {
	PossibleRoute.Add(Maze[CellLocation[ROW] - 1][CellLocation[COL]]);
	PossibleRoute.Add(Maze[CellLocation[ROW] - 2][CellLocation[COL]]);
	return PossibleRoute;
}
TArray<MazeCell> MazeGen::MoveWest(MazeCell** Maze, TArray<MazeCell> PossibleRoute, int* CellLocation) {
	PossibleRoute.Add(Maze[CellLocation[ROW]][CellLocation[COL] - 1]);
	PossibleRoute.Add(Maze[CellLocation[ROW]][CellLocation[COL] - 2]);
	return PossibleRoute;
}
TArray<MazeCell> MazeGen::MoveEast(MazeCell** Maze, TArray<MazeCell> PossibleRoute, int* CellLocation) {
	PossibleRoute.Add(Maze[CellLocation[ROW]][CellLocation[COL] + 1]);
	PossibleRoute.Add(Maze[CellLocation[ROW]][CellLocation[COL] + 2]);
	return PossibleRoute;
}

// reset directions array so all options are available 
static TArray<int> ResetDirections(TArray<int> Directions) {
	int i;
	Directions.Empty();
	for (i = 0; i < 4; i++) {
		Directions.Add(i);
	}
	return Directions;
}

// all possible wall cells are marked as walls at the start, a random floor cell is selected and 
// a random path through the maze is chosen until the selected cell is part of the maze
// then the given path is added to the maze and the walls in that path are 
// dropped to the same height as floor cells  
void MazeGen::DropWalls(MazeCell** Maze, TArray<MazeCell> Cells, int NumCellsX, int NumCellsY) {
	int Rand, index, PrevCellDirection = -1, ImperfectSolutionWalls = 10, RandX, RandY;
	int CellLocation[] = { 0, 0 };
	TArray<MazeCell> PossibleRoute;
	TArray<int> Directions;
	MazeCell TempStart;

	Directions.Init(0, 0);
	PossibleRoute.Init(Maze[0][0], 0);

	//Pick Starting Cell
	Rand = FMath::RandRange(0, Cells.Num() - 1);
	Maze[Cells[Rand].i][Cells[Rand].j].PartOfMaze = true;
	Cells.RemoveAt(Rand);

	while (!Cells.IsEmpty()) {

		//Pick Starting Point For Route
		Rand = FMath::RandRange(0, Cells.Num() - 1);
		TempStart = Maze[Cells[Rand].i][Cells[Rand].j];
		PossibleRoute.Add(TempStart);
		Cells.RemoveAt(Rand);

		while (!Maze[PossibleRoute.Last().i][PossibleRoute.Last().j].PartOfMaze) {
			Directions = ResetDirections(Directions);
			CellLocation[ROW] = PossibleRoute.Last().i;
			CellLocation[COL] = PossibleRoute.Last().j;

			//Most Common Occurrance - cell has 4  directions to choose from
			if (CellLocation[ROW] > 1 && CellLocation[ROW] < NumCellsX - 2 && CellLocation[COL] > 1 && CellLocation[COL] < NumCellsY - 2) {
				//remove no directions
			}
			//North Edge
			else if (CellLocation[ROW] == NumCellsX - 2 && CellLocation[COL] > 1 && CellLocation[COL] < NumCellsY - 2) {
				Directions.Remove(NORTH);
			}
			//South Edge
			else if (CellLocation[ROW] == 1 && CellLocation[COL] > 1 && CellLocation[COL] < NumCellsY - 2) {
				Directions.Remove(SOUTH);
			}
			//West Edge
			else if (CellLocation[COL] == 1 && CellLocation[ROW] > 1 && CellLocation[ROW] < NumCellsX - 2) {
				Directions.Remove(WEST);
			}
			//East Edge
			else if (CellLocation[COL] == NumCellsY - 2 && CellLocation[ROW] > 1 && CellLocation[ROW] < NumCellsX - 2) {
				Directions.Remove(EAST);
			}
			//NorthWest Corner
			else if (CellLocation[ROW] == NumCellsX - 2 && CellLocation[COL] == 1) {
				Directions.Remove(NORTH);
				Directions.Remove(WEST);
			}
			//NorthEast Corner
			else if (CellLocation[ROW] == NumCellsX - 2 && CellLocation[COL] == NumCellsY - 2) {
				Directions.Remove(NORTH);
				Directions.Remove(EAST);
			}
			//SouthWest Corner
			else if (CellLocation[ROW] == 1 && CellLocation[COL] == 1) {
				Directions.Remove(SOUTH);
				Directions.Remove(WEST);
			}
			//SouthEast Corner
			else if (CellLocation[ROW] == 1 && CellLocation[COL] == NumCellsY - 2) {
				Directions.Remove(SOUTH);
				Directions.Remove(EAST);
			}
			//Sanity Check
			else {
				if (GEngine)
					GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Sanity Check Failed In MazeGen::DropWalls"));
			}

			//remove last cell's direction if it hasn't been removed already
			if (Directions.Contains(PrevCellDirection)) {
				Directions.Remove(PrevCellDirection);
			}

			//Select and Move In Direction
			Rand = FMath::RandRange(0, Directions.Num() - 1);
			switch (Directions[Rand]) {
			case NORTH:
				PossibleRoute = MoveNorth(Maze, PossibleRoute, CellLocation);
				PrevCellDirection = SOUTH;
				break;
			case SOUTH:
				PossibleRoute = MoveSouth(Maze, PossibleRoute, CellLocation);
				PrevCellDirection = NORTH;
				break;
			case WEST:
				PossibleRoute = MoveWest(Maze, PossibleRoute, CellLocation);
				PrevCellDirection = EAST;
				break;
			case EAST:
				PossibleRoute = MoveEast(Maze, PossibleRoute, CellLocation);
				PrevCellDirection = WEST;
				break;
			default:
				break;
			}

			//If there is loop in the possible route
			if (CheckForLoop(PossibleRoute)) {
				PossibleRoute.Empty();
				PossibleRoute.Add(TempStart);
				PrevCellDirection = -1;
			}
		}

		//Add Route To Maze
		while (!PossibleRoute.IsEmpty()) {
			Maze[PossibleRoute.Last().i][PossibleRoute.Last().j].CellType = true;
			Maze[PossibleRoute.Last().i][PossibleRoute.Last().j].PartOfMaze = true;
			index = Contains(Cells, PossibleRoute.Last().ObjectID);
			if (index != -1) {
				Cells.RemoveAt(index);
			}
			PossibleRoute.RemoveAt(PossibleRoute.Num() - 1);
		}
	}

	//Drop a few random walls so there can be more than 1 solution
	while (ImperfectSolutionWalls > 0) {
		RandX = FMath::RandRange(1, 25) * 2 - 1;
		RandY = FMath::RandRange(1, 24) * 2;

		if (!Maze[RandX][RandY].PartOfMaze) {
			Maze[RandX][RandY].CellType = true;
			Maze[RandX][RandY].PartOfMaze = true;
			ImperfectSolutionWalls--;
		}
	}
}

// Initialize Maze by setting all possible walls to walls (Each floor cell should be completely surrounded by walls) 
TArray<MazeCell> MazeGen::SetWalls(MazeCell** Maze, TArray<MazeCell> Cells, int Cols, int Rows) {
	int i, j;
	for (i = 0; i < Rows; i++) {
		for (j = 0; j < Cols; j++) {
			Maze[i][j].i = i;
			Maze[i][j].j = j;
			Maze[i][j].ObjectID = i * Rows + j;
			Maze[i][j].PartOfMaze = false;
			if (i % 2 == 1 && j % 2 == 1) {
				Cells.Add(Maze[i][j]);
				Maze[i][j].CellType = true;
			}
			else {
				Maze[i][j].CellType = false;
			}
		}
	}
	return Cells;
}

int** MazeGen::GenerateMaze(int NumCellsX, int NumCellsY) {
	int i, j;
	int** GeneratedMaze;
	FString temp;
	MazeCell** Maze;
	TArray<MazeCell> Cells;

	//initiate generated maze array
	GeneratedMaze = (int**)malloc(NumCellsX * sizeof(int*));
	for (i = 0; i < NumCellsX; i++) {
		*(GeneratedMaze + i) = (int*)malloc(NumCellsY * sizeof(int));
	}

	//initiate struct array for generation
	Maze = (MazeCell**)malloc(NumCellsX * sizeof(MazeCell*));
	for (i = 0; i < NumCellsX; i++) {
		*(Maze + i) = (MazeCell*)malloc(NumCellsY * sizeof(MazeCell));
	}

	Cells.Init(Maze[0][0], 0);
	Cells = SetWalls(Maze, Cells, NumCellsX, NumCellsY);
	DropWalls(Maze, Cells, NumCellsX, NumCellsY);

	//Convert to int array for easy transfer and use
	for (i = 0; i < NumCellsX; i++) {
		for (j = 0; j < NumCellsY; j++) {
			if (Maze[i][j].CellType) {
				GeneratedMaze[i][j] = FLOOR;
			}
			else {
				GeneratedMaze[i][j] = WALL;
			}
		}
	}

	//Set Entrance and Exit to FLOOR cells
	GeneratedMaze[0][25] = FLOOR;
	GeneratedMaze[50][25] = FLOOR;
	
	for (i = 0; i < NumCellsX; i++) {
		for (j = 0; j < NumCellsY; j++) {
			temp += FString::FromInt(GeneratedMaze[i][j]) + " ";
		}
		temp = NULL;
	}

	return GeneratedMaze;
}