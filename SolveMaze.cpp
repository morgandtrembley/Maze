#include "SolveMaze.h"

// return index of Node in Queue with ID 'A'
int GetItem(TArray<QueueStruct> Queue, int A) {
	int Index = 0;
	for (QueueStruct B : Queue) {
		if (A == B.NodeID) {
			return Index;
		}
		Index++;
	}
	return 0;
}

// return Node with ID == Queue Item ID
Node FindNode(TArray<Node> MazeGraph, QueueStruct Item) {
	for (Node A : MazeGraph) {
		if (A.NodeID.Index == Item.NodeID) {
			return A;
		}
	}
	return MazeGraph[0];
}

// if List includes Node 'A' return true
bool Contains(TArray<QueueStruct> List, int A) {
	for (QueueStruct B : List) {
		if (A == B.NodeID) {
			return true;
		}
	}
	return false;
}

// use Complete array to generate exact node path through Maze from 'Start' to 'Finish'
TArray<Node> ConstructRoute(TArray<Node> MazeGraph, TArray<Node> Route, TArray<QueueStruct> Complete, int Start) {
	QueueStruct NodeIndex = Complete.Last();

	do {
		Route.Insert(FindNode(MazeGraph, NodeIndex), 0);
		NodeIndex = Complete[GetItem(Complete, NodeIndex.Parent)];
	} while (Route[0].NodeID.Index != Start);

	return Route;
}

// Remove a Node from a List
void Remove(TArray<QueueStruct>& List, QueueStruct A) {
	for (int i = 0; i < List.Num(); i++) {
		if (A.NodeID == List[i].NodeID) {
			List.RemoveAt(i);
			return;
		}
	}
}

// return Node with Minimum Distance to Finish
QueueStruct Min(TArray<QueueStruct> Queue) {
	QueueStruct Min;

	if (!Queue.IsEmpty()) {
		Min = Queue[0];
		for (QueueStruct Test : Queue) {
			if (Min.TotalDistance > Test.TotalDistance) {
				Min = Test;
			}
		}
	}
	return Min;
}

TArray<Node> SolveMaze::Solve(TArray<Node> MazeGraph, int** Maze, int Start, int Finish) {
	int k, Index;
	bool Exit = false;
	QueueStruct CurrentItem, FinalItem;
	TArray<QueueStruct> Queue, Complete;
	Node CurrentNode;
	TArray<Node> Route;
	
	// initialize Queue
	Queue.Add(CurrentItem);
	Queue.Last().NodeID = MazeGraph[Start].NodeID.Index;
	Queue.Last().TotalDistance = MazeGraph[Start].DistanceToFinish;

	// A* Shortest Path Using Manhattan Distance as weight
	while (!Queue.IsEmpty()) {
		CurrentItem = Min(Queue);
		Remove(Queue, CurrentItem);
		CurrentNode = FindNode(MazeGraph, CurrentItem);

		// for each neighbor in current Node's adj list
		for (k = 0; k < CurrentNode.AdjList.Neighbor.Num(); k++) {
			if (CurrentNode.AdjList.Neighbor[k] != CurrentItem.Parent) {
				if (CurrentNode.AdjList.Neighbor[k] == Finish) {

					// push neighbor as normal to queue and break out of loops
					FinalItem.Parent = CurrentItem.NodeID;
					FinalItem.NodeID = CurrentNode.AdjList.Neighbor[k];
					FinalItem.PathDistance = CurrentItem.PathDistance + CurrentNode.AdjList.Weight[k];
					FinalItem.TotalDistance = FinalItem.PathDistance;

					Exit = true;
					break;
				}

				if (Contains(Queue, CurrentNode.AdjList.Neighbor[k])) {

					//check path distance and replace parent if needed
					Index = GetItem(Queue, CurrentNode.AdjList.Neighbor[k]);
					if (CurrentItem.PathDistance + CurrentNode.AdjList.Weight[k] < Queue[Index].PathDistance) {
						Queue[Index].Parent = CurrentItem.NodeID;
						Queue[Index].PathDistance = CurrentItem.PathDistance + CurrentNode.AdjList.Weight[k];
						Queue[Index].TotalDistance = Queue[Index].PathDistance + MazeGraph[Queue[Index].NodeID].DistanceToFinish;
					}
				}

				if (Contains(Complete, CurrentNode.AdjList.Neighbor[k])) {
					Index = GetItem(Complete, CurrentNode.AdjList.Neighbor[k]);
					if (Complete[Index].TotalDistance <= MazeGraph[CurrentNode.AdjList.Neighbor[k]].DistanceToFinish + CurrentNode.AdjList.Weight[k]) {
						// skip this node
					}
					else {
						Complete[Index].Parent = CurrentItem.NodeID;
						Complete[Index].PathDistance = CurrentItem.PathDistance + CurrentNode.AdjList.Weight[k];
						Complete[Index].TotalDistance = Complete[Index].PathDistance + MazeGraph[CurrentNode.AdjList.Neighbor[k]].DistanceToFinish;
					}
				}
				else if (!Contains(Queue, CurrentNode.AdjList.Neighbor[k])) {
					Queue.Add(CurrentItem);
					Queue.Last().Parent = CurrentItem.NodeID;
					Queue.Last().NodeID = CurrentNode.AdjList.Neighbor[k];
					Queue.Last().PathDistance = CurrentItem.PathDistance + CurrentNode.AdjList.Weight[k];
					Queue.Last().TotalDistance = Queue.Last().PathDistance + MazeGraph[CurrentNode.AdjList.Neighbor[k]].DistanceToFinish;
				}
			}
		}
		Complete.Add(CurrentItem);

		if (Exit) {
			break;
		}
	}

	Complete.Add(FinalItem);

	Route = ConstructRoute(MazeGraph, Route, Complete, Start);

	return Route;
}