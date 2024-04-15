#include <raylib.h>
#include <raymath.h>
#include <rlgl.h>

#include <assert.h>
#include <string.h>
#include <stdio.h>

#define UTILS_IMPL
#include "utils.h"

#define Vec2 Vector2

typedef struct {
  Vec2 pos;
  Vec2 size;
  float speed; // units per second
  KeyboardKey keys[4];
  Color color;
} Player;

int main(void) {
  SetConfigFlags(FLAG_MSAA_4X_HINT);
  SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_WINDOW_RESIZABLE);

  int win_width = 800, win_height = 600;
  InitWindow(win_width, win_height, "Ludum Dare");
  SetTargetFPS(60);
  SetExitKey(KEY_Q);

  Player players[2] = {
    {
      .pos = { 0 },
      .size = { 1, 1, },
      .speed = 1.3,
      .keys = { KEY_W, KEY_A, KEY_S, KEY_D, },
      .color = RED,
    },
    {
      .pos = { 0 },
      .size = { 1, 1, },
      .speed = 1.3,
      .keys = { KEY_K, KEY_H, KEY_J, KEY_L, },
      .color = BLUE,
    },
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
    for (int _ = 0; _ <= 1; _++) {
      Player *player = &players[_];

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
      }
    }
    EndDrawing();

    initial = false;
  }

end:
  CloseWindow();
  return 0;
}
