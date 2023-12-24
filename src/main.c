#include <stdio.h>
#include <stdlib.h>

#include "raylib.h"
#include "raymath.h"

#define SCREEN_WIDTH 1600
#define SCREEN_HEIGHT 900

int main() {
  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Conway's Game of Life");
  SetTargetFPS(60);

  while (!WindowShouldClose()) {
    BeginDrawing();
    {
      ClearBackground(DARKGRAY);
      DrawText("Conway's Game of Life", SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 16,
               WHITE);
    }
    EndDrawing();
  }

  return 0;
}
