#include "GenGraph.h"

// turn 180 degrees
int Flip(int Direction) {
	if (Direction < 2) {
		Direction += 2;
	}
	else {
		Direction -= 2;
	}

	return Direction;
}

// moves target 1 space in given direction
void Shift(int& i, int& j, int Direction) {
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

// find available direction
int FindDirection(int** Maze, int Direction, int i, int j) {
	int k;
	for (k = 0; k < 4; k++) {
		if (k != Direction) {
			Shift(i, j, k);
			if (Maze[i][j] == WALL) {
				Shift(i, j, Flip(k));
			}
			else {
				return k;
			}
		}
	}

	return Direction;
}

int FindNeighbor(int** Maze, Node CurrentNode, int Direction, int& Distance) {
	int i, j, TempDirection;
	i = CurrentNode.NodeID.LocationX;
	j = CurrentNode.NodeID.LocationY;

	// shift in available direction once
	Shift(i, j, Direction);
	Distance++;

	// loop until a neighboring node is found
	do {

		// crawl in selected direction
		while (Maze[i][j] == FLOOR) {
			Shift(i, j, Direction);
			Distance++;
		}

		if (Maze[i][j] == WALL) {

			//Backtrack once
			Direction = Flip(Direction);
			Shift(i, j, Direction);
			TempDirection = FindDirection(Maze, Direction, i, j);
			Distance--;

			//Dead End check
			if (Direction == TempDirection) {
				return CurrentNode.NodeID.Index;
			}
			else {
				Direction = TempDirection;
			}
		}
	} while (Maze[i][j] < 0);

	return Maze[i][j];
}

void ConnectGraph(int** Maze, TArray<Node>& MazeGraph) {
	int k, Neighbor, Distance;
	TArray<int> DirectionsToRemove;

	// iterate through all nodes
	for (k = 0; k < MazeGraph.Num(); k++) {
		// iterate through each available direction
		for (int l : MazeGraph[k].AdjList.Direction) {

			// if 'k' does not already have a neighbor in 'l' direction
			Distance = 0;
			Neighbor = FindNeighbor(Maze, MazeGraph[k], l, Distance);
			if (k == Neighbor) {

				// queue up directions to remove from adj list 
				DirectionsToRemove.Add(l);
			}
			else {
				MazeGraph[k].AdjList.Neighbor.Add(Neighbor);
				MazeGraph[k].AdjList.Weight.Add(Distance);
			}
		}

		// remove directions from adj list
		for (int l : DirectionsToRemove) {
			MazeGraph[k].AdjList.Direction.Remove(l);
		}
		DirectionsToRemove.Empty();
	}
}

void SetNode(TArray<Node>& MazeGraph, Node DefaultGraph, TArray<int> Directions, int i, int j) {
	MazeGraph.Add(DefaultGraph);
	MazeGraph.Last().NodeID.Index = MazeGraph.Num() - 1;
	MazeGraph.Last().NodeID.LocationX = i;
	MazeGraph.Last().NodeID.LocationY = j;
	MazeGraph.Last().AdjList.Direction = Directions;
	MazeGraph.Last().DistanceToFinish = (MAZE_SIZE - i) + (FMath::Abs(MAZE_SIZE / 2 - j));		// Manhattan distance
}

void GenerateVerticies(int** Maze, TArray<Node>& MazeGraph) {
	int i, j;
	NodeID DefaultNodeID;
	AdjacencyList DefaultAdjList;
	Node DefaultNode;
	TArray<int> AvailableDirections;

	DefaultNode.AdjList = DefaultAdjList;
	DefaultNode.NodeID = DefaultNodeID;

	// add starting point
	SetNode(MazeGraph, DefaultNode, { SOUTH }, 0, MAZE_SIZE / 2);
	Maze[MazeGraph.Last().NodeID.LocationX][MazeGraph.Last().NodeID.LocationY] = MazeGraph.Num() - 1;

	// scan maze and add nodes to graph
	for (i = 1; i < MAZE_SIZE; i += 2) {
		for (j = 1; j < MAZE_SIZE; j += 2) {

			// remove overflow directions
			AvailableDirections = { NORTH, EAST, SOUTH, WEST };
			if (Maze[i - 1][j] == WALL) {
				AvailableDirections.Remove(NORTH);
			}
			if (Maze[i + 1][j] == WALL) {
				AvailableDirections.Remove(SOUTH);
			}
			if (Maze[i][j - 1] == WALL) {
				AvailableDirections.Remove(WEST);
			}
			if (Maze[i][j + 1] == WALL) {
				AvailableDirections.Remove(EAST);
			}

			// identify Nodes. Potential nodes with 3+ directions are always junctions, therefore, create node)
			if (AvailableDirections.Num() >= 3) {
				SetNode(MazeGraph, DefaultNode, AvailableDirections, i, j);
				Maze[i][j] = MazeGraph.Last().NodeID.Index;
			}
		}
	}

	// add finishing point
	SetNode(MazeGraph, DefaultNode, { NORTH }, MAZE_SIZE - 1, MAZE_SIZE / 2);
	Maze[MazeGraph.Last().NodeID.LocationX][MazeGraph.Last().NodeID.LocationY] = MazeGraph.Last().NodeID.Index;
}

TArray<Node> GenGraph::Generate(int** Maze) {
	int i, j;
	FString temp;
	TArray<Node> MazeGraph;

	GenerateVerticies(Maze, MazeGraph);
	
	for (i = 0; i < 51; i++) {
		for (j = 0; j < 51; j++) {
			temp += FString::FromInt(Maze[i][j]) + " ";
		}
		temp += "\n";
	}
	UE_LOG(LogTemp, Warning, TEXT("%s"), *temp);

	ConnectGraph(Maze, MazeGraph);

	return MazeGraph;
}