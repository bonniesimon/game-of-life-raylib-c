#include "raylib.h"
#include <raylib.h>
#include <raymath.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

// typedef struct {
//   bool **grid;
//   int rows;
//   int cols;
// };

bool **create_grid(int rows, int cols) {
  bool **grid = (bool **)malloc(rows * sizeof(bool *));

  if (grid == NULL) {
    return NULL;
  }

  for (uint64_t i = 0; i < rows; i++) {
    grid[i] = (bool *)malloc(cols * sizeof(bool));

    if (grid[i] == NULL) {
      for (uint64_t j = 0; j < i; j++) {
        free(grid[j]);
      }
      free(grid);

      return NULL;
    }
  }

  return grid;
}

void fill_grid(bool **grid, int rows, int cols) {
  for (int r = 0; r < rows; r++) {
    for (int c = 0; c < cols; c++) {
      grid[r][c] = rand() % 2 == 0 ? false : true;
    }
  }
}

void print_grid(bool **grid, int rows, int cols) {
  for (int r = 0; r < rows; r++) {
    for (int c = 0; c < cols; c++) {
      printf("%d  ", grid[r][c]);
    }
    printf("\n");
  }
}

void update_state(){};
void draw_state(bool **grid, int rows, int cols) {
  for (int r = 0; r < rows; r++) {
    for (int c = 0; c < cols; c++) {
      if (grid[r][c] == true) {
        int scale = 10;
        DrawRectangle(r * scale, c * scale, scale, scale, YELLOW);
      }
    }
  }
};

int main() {
  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Conway's Game of Life");
  SetTargetFPS(60);

  int grid_width = SCREEN_WIDTH / 10;
  int grid_height = SCREEN_HEIGHT / 10;

  bool **grid = create_grid(grid_height, grid_width);
  // print_grid(grid, grid_height, grid_width);
  fill_grid(grid, grid_height, grid_width);

  while (!WindowShouldClose()) {
    BeginDrawing();
    {
      ClearBackground(DARKGRAY);
      update_state();
      draw_state(grid, grid_height, grid_width);
    }
    EndDrawing();
  }

  return 0;
}
