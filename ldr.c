#include <raylib.h>
#include <raymath.h>
#include <rlgl.h>

#include <assert.h>
#include <string.h>
#include <stdio.h>

#define UTILS_IMPL
#include "utils.h"

int main(void) {
  SetConfigFlags(FLAG_MSAA_4X_HINT);
  SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_WINDOW_RESIZABLE);

  int win_width = 800, win_height = 600;
  InitWindow(win_width, win_height, "Ludum Dare");
  SetTargetFPS(60);
  SetExitKey(KEY_Q);

  bool initial = true; // This is a terrible solution

  Camera2D camera = { 0 };

  while (!WindowShouldClose()) {
    if (IsWindowResized() || initial) {
      win_width = GetScreenWidth();
      win_height = GetScreenHeight();
    }

    /* ===== Input ===== */
    {
    }

    /* ===== Drawing ===== */
    BeginDrawing();
    ClearBackground(BLACK);
    BeginMode2D(camera);
    {

    }
    EndMode2D();
    EndDrawing();

    initial = false;
  }

end:
  CloseWindow();
  return 0;
}
