This program creates and solves a 50 by 50 maze. The player can decide to display the solution and their progress through the maze is
  tracked if the solution is displayed.

Definitions is the collection of structs and definitions I used throughout the program. 

GenGraph generates a graph based on the maze.

MazeGen generates a maze the game mode base will use to construct the maze in the level.

MazeSegment is an actor that represents one cell in the maze.

SolveMaze uses A* search and the graph to return the shortest path through the maze.
