#include <raylib.h>
#include <raymath.h>
#include <rlgl.h>

#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <time.h>

#define UTILS_IMPL
#include "utils.h"

#include "maze.h"

#define Vec2 Vector2

#define ENTITY_F() \
  int x, y; \
  Vec2 size; \
  float health; \
  Color color; \
  float timer; \
  float tps

typedef struct {
  ENTITY_F();
  KeyboardKey keys[4];
} Player;

typedef struct {
  ENTITY_F();
  Vec2I dir;
  enum {
    ENEMY_GUNMAN,
    NUM_ENEMIES,
  } type;
} Enemy;

typedef struct {
  ENTITY_F();
  Vec2I dir;
  bool freed;
  float damage;
} Bullet;

Enemy enemy_defs[NUM_ENEMIES] = {
  [ENEMY_GUNMAN] = {
    .size = { 1.0, 1.0, },
    .health = 1.0,
    .color = { 255, 255, 0, 255, },
    .timer = 1.0,
    .tps = 1.0,
  },
};

int main(void) {
  srand(time(0));

  SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_WINDOW_RESIZABLE);

  int win_width = 800, win_height = 600;
  InitWindow(win_width, win_height, "Ludum Dare");
  SetTargetFPS(60);
  SetExitKey(KEY_Q);

  Maze maze;
  gen_maze(&maze, 200, 200);
  maze.data[maze.w/2][maze.h/2] = FLOOR;

  Player players[2] = {
    {
      .x = maze.w/2,
      .y = maze.h/2,
      .size = { 0.9, 0.9, },
      .keys = { KEY_W, KEY_A, KEY_S, KEY_D, },
      .color = RED,
      .health = 1.0,
      .tps = 5.0,
    },
    {
      .x = maze.w/2,
      .y = maze.h/2,
      .size = { 0.9, 0.9, },
      .keys = { KEY_K, KEY_H, KEY_J, KEY_L, },
      .color = BLUE,
      .health = 1.0,
      .tps = 5.0,
    },
  };
  int summoner_index = 0;

  List(Enemy) enemies = { .cap = 256, };
  da_init(&enemies);
  da_push(&enemies, enemy_defs[ENEMY_GUNMAN]);
  da_last(&enemies).x = maze.w/2;
  da_last(&enemies).y = maze.h/2;
  da_last(&enemies).dir = (Vec2I) { 1, 0, };

  Pool(Bullet) bullets;
  pool_init(&bullets);

  bool initial = true; // This is a terrible solution
  int unit = 0;
  float zoom = 0.1;

  while (!WindowShouldClose()) {
    if (IsWindowResized() || initial) {
      win_width = GetScreenWidth();
      win_height = GetScreenHeight();
      unit = min(
        zoom * win_width,
        zoom * win_height
      );
    }

    float delta = GetFrameTime();

    BeginDrawing();
    ClearBackground(BLACK);

    Camera2D camera = {
      .target = {
        .x = (players[0].x + players[1].x) * 0.5 * unit,
        .y = (players[0].y + players[1].y) * 0.5 * unit,
      },
      .offset = {
        min(win_width, win_height)/2,
        min(win_width, win_height)/2,
      },
      .zoom = 1.0,
    };

    BeginMode2D(camera);
    for (int _ = 0; _ <= 1; _++) {
      Player *player = &players[_];

      /* ===== Drawing ===== */
      {
        for (int i = 0; i < bullets.count; i++) {
          Bullet *bullet = &bullets.items[i];
          DrawCircle(
            unit * bullet->x + unit/2,
            unit * bullet->y + unit/2,
            unit * 0.1,
            RED
          );
        }

        DrawRectangle(
          unit * player->x,
          unit * player->y,
          unit * player->size.x,
          unit * player->size.y,
          player->color
        );

        for (int i = 0; i < enemies.count; i++) {
          Enemy *enemy = &enemies.items[i];
          DrawRectangle(
            unit * enemy->x,
            unit * enemy->y,
            unit * enemy->size.x,
            unit * enemy->size.y,
            enemy->color
          );
        }

        for (int x = 0; x < maze.w; x++) {
          for (int y = 0; y < maze.h; y++) {
            DrawRectangle(
              unit * x,
              unit * y,
              unit, unit,
              maze_colors[maze.data[x][y]]
            );
          }
        }
      }

      /* ===== Input ===== */
      {
        Vec2I vel = { 0 };

        if (player->timer <= 0) {
          if (IsKeyDown(player->keys[0])) vel.y = -1;
          if (IsKeyDown(player->keys[1])) vel.x = -1;
          if (IsKeyDown(player->keys[2])) vel.y = +1;
          if (IsKeyDown(player->keys[3])) vel.x = +1;

          player->timer = 1/player->tps;
        } else {
          player->timer -= delta;
        }

        if (IsKeyPressed(player->keys[0])) vel.y = -1;
        if (IsKeyPressed(player->keys[1])) vel.x = -1;
        if (IsKeyPressed(player->keys[2])) vel.y = +1;
        if (IsKeyPressed(player->keys[3])) vel.x = +1;

        if (maze.data[player->x + vel.x][player->y] != WALL) {
          player->x += vel.x;
        }
        if (maze.data[player->x][player->y + vel.y] != WALL) {
          player->y += vel.y;
        }
      }

      /* ===== Simulation ===== */
      {
        {
          if (player->health <= 0) {
            printf("You lose hahaha >:()\n");
            exit(1);
          }
        }

        for (int i = 0; i < enemies.count; i++) {
          Enemy *enemy = &enemies.items[i];
          enemy->timer -= delta;

          if (enemy->timer <= 0) {
            Bullet bullet = {
              .x = enemy->x,
              .y = enemy->y,
              .dir = enemy->dir,
              .tps = 10.0,
              .damage = 0.1,
            };
            pool_add(&bullets, bullet);

            enemy->timer = 1/enemy->tps;
          }
        }

        for (int i = 0; i < bullets.count; i++) {
          Bullet *bullet = &bullets.items[i];
          if (bullet->freed) continue;
          bullet->timer -= delta;

          if (bullet->timer <= 0) {
            bullet->x += bullet->dir.x;
            bullet->y += bullet->dir.y;

            bullet->timer = 1/bullet->tps;
          }

          if (maze.data[bullet->x][bullet->y] == WALL) {
            bullet->freed = true;
            pool_del(&bullets, i);
            continue;
          }

          if (bullet->x == player->x && bullet->y == player->y) {
            player->health -= bullet->damage;
            bullet->freed = true;
            pool_del(&bullets, i);
            continue;
          }
        }
      }
    }
    EndMode2D();
    EndDrawing();

    initial = false;
  }

  CloseWindow();
  return 0;
}
