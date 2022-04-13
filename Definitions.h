#pragma once
#include "CoreMinimal.h"

// maze definitions
#define MAZE_SIZE 51
#define FLOOR -1
#define WALL -2
#define ROW 0
#define COL 1

// maze solution definitions
#define INT_MAX 2147483647
#define SOLUTION 88
#define NORTH 0
#define EAST 1
#define SOUTH 2
#define WEST 3

// colors for drawn solution in game
#define RED 0.5,0,0
#define GREEN 0,1,0
#define BLUE 0,0.44,0.5

// necessary info for solving algorithm
struct NodeID {
	int Index = -1;
	int LocationX = 0;
	int LocationY = 0;
};

// adjacency list for a Node
struct AdjacencyList {
	TArray<int> Neighbor;
	TArray<int> Direction;
	TArray<int> Weight;
};

// TArray<Node> holds every node along with its Adj list and distance to finish
struct Node {
	NodeID NodeID;
	AdjacencyList AdjList;
	int DistanceToFinish = INT_MAX;
};

// Queue for A* solve algorithm
struct QueueStruct {
	int NodeID = -1;
	int Parent = -1;
	int PathDistance = INT_MAX;
	int TotalDistance = INT_MAX;
};

struct MazeCell {

	//coordinates in Graph
	int i;
	int j;
	int ObjectID;
	bool PartOfMaze;

	//whether cell is wall (0) or floor (1)
	bool CellType;
};

class MAZE_API Definitions {
public:
	Definitions();
};
