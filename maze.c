#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "maze.h"

static void maze_hline(Maze *maze, int x, int y, int l, Block block) {
  for (int i = 0; i < l; i++)
    maze->data[x + i][y] = block;
}

static void maze_vline(Maze *maze, int x, int y, int l, Block block) {
  for (int i = 0; i < l; i++)
    maze->data[x][y + i] = block;
}

static void maze_rect(Maze *maze, int x, int y, int w, int h, Block type) {
  for (int i = 0; i < w; i++) {
    for (int j = 0; j < h; j++) {
      maze->data[x + i][y + j] = FLOOR;
    }
  }
}

static int maze_check(Maze *maze, int x, int y, int w, int h) {
  for (int i = 0; i < w; i++) {
    for (int j = 0; j < h; j++) {
      if (maze->data[x + i][y + j] != 0)
        return 0;
    }
  }
  return 1;
}

static void maze_box(Maze *maze, int x, int y, int w, int h) {
  maze_hline(maze, x, y, w, PIT);
  maze_hline(maze, x, y + h - 1, w, PIT);
  maze_vline(maze, x + w - 1, y + 1, h - 2, PIT);
  maze_vline(maze, x, y + 1, h - 2, PIT);
  maze_rect(maze, x + 1, y + 1, w - 2, h - 2, FLOOR);
}

static void maze_gen_path(Maze *maze, int x, int y) {
  Block cur = maze->data[x][y];
  if (cur != PIT && cur != 0)
    return;

  maze->data[x][y] = FLOOR;

  int dirs = rand();
  int up = dirs & 1;
  int down = dirs & 2;
  int left = dirs & 4;
  int right = dirs & 8;

  if (up)
    maze_gen_path(maze, x, y + 1);
  else if (maze->data[x][y + 1] == 0)
    maze->data[x][y + 1] = WALL;

  if (down)
    maze_gen_path(maze, x, y - 1);
  else if (maze->data[x][y - 1] == 0)
    maze->data[x][y - 1] = WALL;

  if (left)
    maze_gen_path(maze, x - 1, y);
  else if (maze->data[x - 1][y] == 0)
    maze->data[x - 1][y] = WALL;

  if (right)
    maze_gen_path(maze, x + 1, y);
  else if (maze->data[x + 1][y] == 0)
    maze->data[x + 1][y] = WALL;
}

void gen_maze(Maze *maze, int x, int y) {
  assert(x <= MAX_MAZE_SIZE);
  assert(y <= MAX_MAZE_SIZE);

  memset(maze->data, 0, sizeof(maze->data));

  maze->w = x;
  maze->h = y;

  for (int i = 0; i < x * y; i++){
    int w = rand() % 8 + 1;
    w = (1.0 / (w * w)) * 32 + 3;

    int h = rand() % 8 + 1;
    h = (1.0 / (h * h)) * 32 + 3;

    for (int i = 0; i < 10; i++){
     int X = rand() % x;
     int Y = rand() % y;

      if (maze_check(maze, X-2, Y-2, w+4, h+4)) {
        maze_box(maze, X, Y, w, h);
        break;
      }
    }
  }


  for (int i = 0; i < x; i++)
    for (int j = 0; j < y; j++)
      maze_gen_path(maze, i, j);
}

void print_maze(Maze *maze) {
  for(int i = 0; i < maze->h; i++){
    for(int j = 0; j < maze->w; j++){
      int cur = maze->data[j][i];

      printf("%c%c", "@# ~"[cur], "@# ~"[cur]);
    }
    printf("\n");
  }
}

// int main(){
//   struct Maze maze = gen_maze(80, 80);
//   print_maze(maze);
//   return 0;
// }
