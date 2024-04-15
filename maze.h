#ifndef MAZE_H
#define MAZE_H

typedef struct {
  char *data;
  int wb, w, h;
} Maze;

enum {
  EMPTY = 0,
  WALL = 1,
  FLOOR = 2,
  PIT = 3
};

int maze_get(Maze maze, int x, int y);
void maze_set(Maze maze, int x, int y, int v);
Maze gen_maze(int x, int y);

#endif // MAZE_H
