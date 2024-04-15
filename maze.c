#include<stdio.h>
#include<stdlib.h>

typedef struct Maze {
  char *data;
  int wb, w, h;
} Maze;

enum{
  EMPTY = 0,
  WALL = 1,
  FLOOR = 2,
  PIT = 3
};

int maze_get(Maze maze, int x, int y){
  if(x < 0 || x >= maze.w || y < 0 || y >= maze.h)
    return WALL;

  return (maze.data[x/4 + y*maze.wb] >> ((x & 3) * 2)) & 3;
}

void maze_set(Maze maze, int x, int y, int v){
  if(x < 0 || x >= maze.w || y < 0 || y >= maze.h)
    return;

  int offset = (x & 3) * 2; 
  maze.data[x/4 + y*maze.wb] &= ~(3 << offset);
  maze.data[x/4 + y*maze.wb] |= (v & 3) << offset;
}

void maze_hline(Maze maze, int x, int y, int l, int type){
  for(int i = 0; i < l; i++)
    maze_set(maze, x + i, y, type);
}

void maze_vline(Maze maze, int x, int y, int l, int type){
  for(int i = 0; i < l; i++)
    maze_set(maze, x, y + i, type);
}

void maze_rect(Maze maze, int x, int y, int w, int h, int type){
  for(int i = 0; i < w; i++){
    for(int j = 0; j < h; j++){
      maze_set(maze, x + i, y + j, FLOOR);
    }
  }
}

int maze_check(Maze maze, int x, int y, int w, int h){
  for(int i = 0; i < w; i++){
    for(int j = 0; j < h; j++){
      if(maze_get(maze, x + i, y + j) != 0)
        return 0;
    }
  }
  return 1;
}

void maze_box(Maze maze, int x, int y, int w, int h){
  maze_hline(maze, x, y, w, PIT);
  maze_hline(maze, x, y + h - 1, w, PIT);
  maze_vline(maze, x + w - 1, y + 1, h - 2, PIT);
  maze_vline(maze, x, y + 1, h - 2, PIT);
  maze_rect(maze, x + 1, y + 1, w - 2, h - 2, FLOOR);
}

void maze_gen_path(Maze maze, int x, int y){
  int cur = maze_get(maze, x, y);
  if(cur != PIT && cur != 0)
    return;

  maze_set(maze, x, y, FLOOR);

  int dirs = rand();
  int up = dirs & 1;
  int down = dirs & 2;
  int left = dirs & 4;
  int right = dirs & 8;

  if(up)
    maze_gen_path(maze, x, y + 1);
  else if(maze_get(maze, x, y + 1) == 0)
    maze_set(maze, x, y + 1, WALL);

  if(down)
    maze_gen_path(maze, x, y - 1);
  else if(maze_get(maze, x, y - 1) == 0)
    maze_set(maze, x, y - 1, WALL);
  
  if(left)
    maze_gen_path(maze, x - 1, y);
  else if(maze_get(maze, x - 1, y) == 0)
    maze_set(maze, x - 1, y, WALL);

  if(right)
    maze_gen_path(maze, x + 1, y);
  else if(maze_get(maze, x + 1, y) == 0)
    maze_set(maze, x + 1, y, WALL);
}

Maze gen_maze(int x, int y){
  struct Maze maze = {
    .data = calloc(sizeof(char) * (x/4 + 1) * y, 1),
    .wb = (x/4 + 1),
    .w = x,
    .h = y
  };

  for(int i = 0; i < x * y; i++){
    int w = rand() % 10 + 1;
    printf("%d\n", w);
    w = (1.0 / (w * w)) * 32 + 8;

    int h = rand() % 10 + 1;
    printf("%d\n", h);
    h = (1.0 / (h * h)) * 32 + 8;

    printf("%d %d\n", w, h);
    for(int i = 0; i < 10; i++){
     int X = rand() % x;
     int Y = rand() % y;

    if(maze_check(maze, X-1, Y-1, w+2, h+2)){
      maze_box(maze, X, Y, w, h);
      break;
    }
    }
  }


  for(int i = 0; i < x; i++)
    for(int j = 0; j < y; j++)
      maze_gen_path(maze, i, j);


  return maze;
}

void print_maze(struct Maze maze){
  for(int i = 0; i < maze.h; i++){
    for(int j = 0; j < maze.w; j++)
      printf("%d", maze_get(maze, j, i));
    printf("\n");
  }
}

/*
int main(){
  struct Maze maze = gen_maze(80, 80);
  print_maze(maze);
  return 0;
}
*/
