#include <raylib.h>
#include <raymath.h>
#include <rlgl.h>

#include <assert.h>
#include <string.h>
#include <stdio.h>

#define UTILS_IMPL
#include "utils.h"

#include "maze.h"

#define Vec2 Vector2

#define ENTITY_F() \
  Vec2 pos; \
  Vec2 size; \
  float speed; \
  float health; \
  Color color

typedef struct {
  ENTITY_F();
  KeyboardKey keys[4];
} Player;

typedef struct {
  ENTITY_F();
  float dir;
  enum {
    ENEMY_GUNMAN,
    NUM_ENEMIES,
  } type;
} Enemy;

Enemy enemy_defs[NUM_ENEMIES] = {
  [ENEMY_GUNMAN] = {
    .size = { 1.0, 1.0, },
    .speed = 1.0,
    .health = 1.0,
    .color = { 255, 255, 0, 255, },
  },
};

int main(void) {
  SetConfigFlags(FLAG_MSAA_4X_HINT);
  SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_WINDOW_RESIZABLE);

  int win_width = 800, win_height = 600;
  InitWindow(win_width, win_height, "Ludum Dare");
  SetTargetFPS(60);
  SetExitKey(KEY_Q);

  // Maze maze = gen_maze(0, 0);

  Player players[2] = {
    {
      .pos = { 0 },
      .size = { 0.9, 0.9, },
      .speed = 2.2,
      .keys = { KEY_W, KEY_A, KEY_S, KEY_D, },
      .color = RED,
      .health = 1.0,
    },
    {
      .pos = { 0 },
      .size = { 0.9, 0.9, },
      .speed = 2.2,
      .keys = { KEY_K, KEY_H, KEY_J, KEY_L, },
      .color = BLUE,
      .health = 1.0,
    },
  };
  int summoner_index = 0;

  float map_width = 10, map_height = 10;

  List(Enemy) enemies = { .cap = 256, };
  da_init(&enemies);
  da_push(&enemies, enemy_defs[ENEMY_GUNMAN]);
  da_last(&enemies).pos = (Vec2) {
    .x = randf(0, map_width),
    .y = randf(0, map_height),
  };

  bool initial = true; // This is a terrible solution
  float unit = 0;

  while (!WindowShouldClose()) {
    if (IsWindowResized() || initial) {
      win_width = GetScreenWidth();
      win_height = GetScreenHeight();
      unit = min(
        0.1 * win_width,
        0.1 * win_height
      );
    }

    float delta = GetFrameTime();

    BeginDrawing();
    ClearBackground(BLACK);

    Camera2D camera = {
      .target = {
        .x = (players[0].pos.x + players[1].pos.x) * 0.5 * unit,
        .y = (players[0].pos.y + players[1].pos.y) * 0.5 * unit,
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

      /* ===== Simulation ===== */
      {
        // for (int i = 0; i < enemies.count; i++) {
        //   Enemy *enemy = &enemies.items[i];

        //   enemy->pos.x += cos(enemy->dir) * enemy->speed * delta;
        //   enemy->pos.y += sin(enemy->dir) * enemy->speed * delta;

        //   int follow_index = Vector2Distance(enemy->pos, players[0].pos) <
        //                      Vector2Distance(enemy->pos, players[1].pos) ?
        //                      0 : 1;
        //   enemy->dir = Vector2Angle(enemy->pos, players[follow_index].pos);
        // }
      }

      /* ===== Input ===== */
      {
        Vec2 vel = { 0 };

        if (IsKeyDown(player->keys[0])) vel.y -= delta * player->speed;
        if (IsKeyDown(player->keys[1])) vel.x -= delta * player->speed;
        if (IsKeyDown(player->keys[2])) vel.y += delta * player->speed;
        if (IsKeyDown(player->keys[3])) vel.x += delta * player->speed;

        player->pos.x += vel.x;
        player->pos.y += vel.y;
      }

      /* ===== Drawing ===== */
      {
        DrawRectangle(
          unit * player->pos.x,
          unit * player->pos.y,
          unit * player->size.x,
          unit * player->size.y,
          player->color
        );

        for (int i = 0; i < enemies.count; i++) {
          Enemy *enemy = &enemies.items[i];
          DrawRectangle(
            unit * enemy->pos.x,
            unit * enemy->pos.y,
            unit * enemy->size.x,
            unit * enemy->size.y,
            enemy->color
          );
        }

        // for (int gh)
      }
    }
    EndMode2D();
    EndDrawing();

    initial = false;
  }

end:
  CloseWindow();
  return 0;
}
