#include "MazeGameModeBase.h"

int Reverse(int Direction) {
	if (Direction < 2) {
		Direction += 2;
	}
	else {
		Direction -= 2;
	}
	return Direction;
}

// move once in given direction
void Move(int& i, int& j, int Direction) {
	switch (Direction) {
	case NORTH:
		i--;
		break;
	case EAST:
		j++;
		break;
	case SOUTH:
		i++;
		break;
	case WEST:
		j--;
		break;
	default:
		break;
	}
}

// iterate through directly neighboring cells and return direction if it is a floor cell
int FindDir(int** Maze, int i, int j) {
	if (Maze[i][j - 1] == FLOOR) {
		return WEST;
	}
	else if (Maze[i][j + 1] == FLOOR) {
		return EAST;
	}
	else if (Maze[i - 1][j] == FLOOR) {
		return NORTH;
	}
	else {
		return SOUTH;
	}
}

// Walk from 'Start' in 'Direction' until 'Finish' is found, mark each cell on the way as part of the solution
void DrawPath(int** Maze, Node Start, Node Finish, int Direction) {
	int i, j;

	i = Start.NodeID.LocationX;
	j = Start.NodeID.LocationY;
	Maze[i][j] = SOLUTION;
	Move(i, j, Direction);

	while (!(i == Finish.NodeID.LocationX && j == Finish.NodeID.LocationY)) {
		Maze[i][j] = SOLUTION;

		Direction = FindDir(Maze, i, j);
		Move(i, j, Direction);
	}
}

// for each Node in Route, draw a path to the solution
void DrawRoute(TArray<Node> Route, int** Maze) {
	int i, j, DirectionToNextNode, TempWeight;
	Node CurrentNode, NextNode;

	// for each Node in the solution route
	for (i = 1; i < Route.Num(); i++) {
		// i - 1 protects from overflow on final Node
		CurrentNode = Route[i - 1];
		NextNode = Route[i];

		// ensure at least 1 neighbor has a better direction and weight 
		DirectionToNextNode = -1;
		TempWeight = INT_MAX;

		// for each Node in Current Node's Adj list (guarantees min weight neighbor is explored first)
		for (j = 0; j < CurrentNode.AdjList.Neighbor.Num(); j++) {

			// if Neighbor is the next Node in Route and has lower weight
			if (CurrentNode.AdjList.Neighbor[j] == NextNode.NodeID.Index) {
				if (CurrentNode.AdjList.Weight[j] < TempWeight) {

					// set temp direction and weight
					DirectionToNextNode = CurrentNode.AdjList.Direction[j];
					TempWeight = CurrentNode.AdjList.Weight[j];
				}
			}
		}

		// add path to solution
		DrawPath(Maze, CurrentNode, NextNode, DirectionToNextNode);
	}

	// add final node to solution
	Maze[Route.Last().NodeID.LocationX][Route.Last().NodeID.LocationY] = SOLUTION;
}

// resets any marked cells back to original Maze input
void CleanMaze(int** Maze) {
	int i, j;
	for (i = 0; i < MAZE_SIZE; i++) {
		for (j = 0; j < MAZE_SIZE; j++) {
			if (Maze[i][j] >= 0) {
				Maze[i][j] = -1;
			}
		}
	}
}

// Reset Level objects to original Location and underglow colors
void AMazeGameModeBase::ResetMaze() {
	int i;
	TArray<UStaticMeshComponent*> SegmentComponents;

	for (i = 0; i < Maze.Num(); i++) {
		//drop walls to floor height
		Maze[i]->VisualMesh->SetRelativeLocation(FVector(0, 0, -145));

		//reset color of floor
		Maze[i]->GetComponents(SegmentComponents);
		SegmentComponents[0]->SetVectorParameterValueOnMaterials(FName("Color"), FVector(BLUE));			//Desired mesh should always be at  SegmentComponents[0]
		SegmentComponents[1]->SetVectorParameterValueOnMaterials(FName("Color"), FVector(0.25,0.25,0.25));
		SegmentComponents.Empty();
	}

}

// Toggle Solution Display in Level
void AMazeGameModeBase::ShowSolution() {
	int i, j;

	// component with changable material color
	TArray<UStaticMeshComponent*> SegmentComponents;

	// for each cell in Maze
	for (i = 0; i < MAZE_SIZE; i++) {
		for (j = 0; j < MAZE_SIZE; j++) {
			if (GeneratedMaze[i][j] == SOLUTION) {

				// get component for cell
				Maze[i * MAZE_SIZE + j]->GetComponents(SegmentComponents);

				// toggle color of cell
				if (SolutionShown) {
					SegmentComponents[0]->SetVectorParameterValueOnMaterials(FName("Color"), FVector(BLUE));			//Desired mesh should always be at  SegmentComponents[0]
				}
				else {
					SegmentComponents[0]->SetVectorParameterValueOnMaterials(FName("Color"), FVector(GREEN));
				}
				SegmentComponents.Empty();		
			}
		}
	}

	// set toggle for next function call
	SolutionShown = !SolutionShown;
}

// set all walls underglow components to red and all floor underglow components to blue
void AMazeGameModeBase::SetWallColor() {
	int i;
	TArray<UStaticMeshComponent*> SegmentComponents;

	for (i = 0; i < Maze.Num(); i++) {

		// if level object location is above 0, it is a wall
		if (Maze[i]->VisualMesh->GetComponentLocation().Z > 0) {
			Maze[i]->GetComponents(SegmentComponents);
			SegmentComponents[0]->SetVectorParameterValueOnMaterials(FName("Color"), FVector(RED));
			SegmentComponents[1]->SetVectorParameterValueOnMaterials(FName("Color"), FVector(0, 0, 0));
			SegmentComponents.Empty();
		}
		else {
			Maze[i]->GetComponents(SegmentComponents);
			SegmentComponents[0]->SetVectorParameterValueOnMaterials(FName("Color"), FVector(BLUE));
			SegmentComponents.Empty();
		}
	}
}

//generates a physical representation of generated maze in level
void AMazeGameModeBase::ConstructMaze() {
	for (int i = 0; i < MAZE_SIZE; i++) {
		for (int j = 0; j < MAZE_SIZE; j++) {
			if (GeneratedMaze[i][j] == WALL) {
				Maze[i * MAZE_SIZE + j]->VisualMesh->AddRelativeLocation(FVector(0, 0, 200));
			}
		}
	}

	SetWallColor();

	// toggle solution display off for newly constructed maze
	if (SolutionShown) {
		SolutionShown = false;
		ShowSolution();
	}
}

// generate a maze, it's node graph, set start and finish, and solve it
void AMazeGameModeBase::PrepMaze() {
	GeneratedMaze = MazeGen::GenerateMaze(MAZE_SIZE, MAZE_SIZE);
	MazeGraph = GenGraph::Generate(GeneratedMaze);
	int Start = 0, Finish = MazeGraph.Last().NodeID.Index;
	Solution = SolveMaze::Solve(MazeGraph, GeneratedMaze, Start, Finish);
	CleanMaze(GeneratedMaze);
	DrawRoute(Solution, GeneratedMaze);
}

//spawns floor and populates Maze array
void AMazeGameModeBase::InitPlayArea() {
	int i, j;
	FVector Location(0, 6000, 0);
	FRotator Rotation(0, 0, 0);
	FActorSpawnParameters SpawnInfo;

	for (i = 0; i < MAZE_SIZE + 2 * WalkwaySize; i++) {
		Location.Y = 6000;
		for (j = MAZE_SIZE + 2 * WalkwaySize - 1; j >= 0; j--) {
			NewMazeCell = GetWorld()->SpawnActor<AMazeSegment>(Location, Rotation, SpawnInfo);

			if (i >= WalkwaySize && j >= WalkwaySize && i < MAZE_SIZE + WalkwaySize && j < MAZE_SIZE + WalkwaySize) {
				Maze.Add(NewMazeCell);
			}
			Location.Y -= 100;
		}
		Location.X += 100;
	}
}

AMazeGameModeBase::AMazeGameModeBase() {
	PrimaryActorTick.bCanEverTick = true;
}

void AMazeGameModeBase::BeginPlay() {
	Super::BeginPlay();
	
	InitPlayArea();
	PrepMaze();
	ConstructMaze();
}