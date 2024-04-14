#include <raylib.h>
#include <raymath.h>
#include <rlgl.h>

#include <assert.h>
#include <string.h>
#include <stdio.h>

#define UTILS_IMPL
#include "utils.h"

#define UI_IMPL
#include "ui.h"

int main(void) {
  SetConfigFlags(FLAG_MSAA_4X_HINT);
  SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_WINDOW_RESIZABLE);

  InitWindow(800, 600, "Ludum Dare");
  SetTargetFPS(60);
  SetExitKey(KEY_Q);

  bool initial = true; // This is a terrible solution

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
    BeginMode2D();
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
