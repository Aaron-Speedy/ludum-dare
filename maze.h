#ifndef MAZE_H
#define MAZE_H

#include "raylib.h"

typedef struct {
  char *data;
  int wb, w, h;
} Maze;

enum {
  EXTRA = 0,
  WALL,
  FLOOR,
  PIT,
  NUM_BLOCK_TYPES,
};

static Color maze_colors[NUM_BLOCK_TYPES] = {
  [WALL] = { 255, 255, 255, 255, },
  [FLOOR] = { 0 },
  [PIT] = { 0 },
};

int maze_get(Maze maze, int x, int y);
void maze_set(Maze maze, int x, int y, int v);

Maze gen_maze(int x, int y);

void maze_free(Maze maze);

void print_maze(Maze maze);

#endif // MAZE_H
