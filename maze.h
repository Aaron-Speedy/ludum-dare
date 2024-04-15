#ifndef MAZE_H
#define MAZE_H

#include "raylib.h"

typedef enum {
  EXTRA = 0,
  WALL,
  FLOOR,
  PIT,
  NUM_BLOCK_TYPES,
} Block;

static Color maze_colors[NUM_BLOCK_TYPES] = {
  [WALL] = { 100, 100, 100, 255, },
  [FLOOR] = { 0 },
  [PIT] = { 0 },
};

#define MAX_MAZE_SIZE 500

typedef struct {
  Block data[MAX_MAZE_SIZE][MAX_MAZE_SIZE];
  int w, h;
} Maze;

void gen_maze(Maze *maze, int x, int y);

void print_maze(Maze *maze);

#endif // MAZE_H
