#include "raylib.h"
#include <raylib.h>
#include <raymath.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

typedef struct {
  bool **grid;
  int rows;
  int cols;
} Board;

int neighbors[][2] = {{-1, -1}, {0, -1}, {1, -1}, {-1, 0},
                      {1, 0},   {-1, 1}, {0, 1},  {1, 1}};

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

void fill_grid(Board *board) {
  for (int r = 0; r < board->rows; r++) {
    for (int c = 0; c < board->cols; c++) {
      board->grid[r][c] = GetRandomValue(0, 9) > 8 ? true : false;
    }
  }

  // board->grid[1][1] = true;
  // board->grid[2][1] = true;
  // board->grid[3][1] = true;
  // board->grid[2][2] = true;
}

void print_grid(Board *board) {
  for (int r = 0; r < board->rows; r++) {
    for (int c = 0; c < board->cols; c++) {
      printf("%d  ", board->grid[r][c]);
    }
    printf("\n");
  }
}

int calculate_neighbor_sum(Board *board, int row, int col) {
  int sum = 0;

  for (int x = 0; x < 8; x++) {
    int new_row = row + neighbors[x][0];
    int new_col = col + neighbors[x][1];

    if (new_col < 0 || new_row < 0 || new_col > board->cols - 1 ||
        new_row > board->rows - 1) {
      continue;
    }

    sum = sum + board->grid[new_row][new_col];
  }

  if (row == 1 && col == 1) {
    printf("sum = %d\n", sum);
  }

  return sum;
}

bool is_underpopulated(Board *board, int row, int col) {
  int sum = calculate_neighbor_sum(board, row, col);

  return sum < 2;
}

bool is_new_generation_possible(Board *board, int row, int col) {
  int sum = calculate_neighbor_sum(board, row, col);

  return sum == 2 || sum == 3;
}

bool is_overpopulated(Board *board, int row, int col) {
  int sum = calculate_neighbor_sum(board, row, col);

  return sum > 3;
}

bool is_reproduction(Board *board, int row, int col) {
  int sum = calculate_neighbor_sum(board, row, col);

  return sum == 3;
}

void update_state(Board *board) {
  printf("------------------------------\n\n");
  for (int r = 0; r < board->rows; r++) {
    for (int c = 0; c < board->cols; c++) {
      printf("%d, %d: %d\n", r, c, board->grid[r][c]);
      if (board->grid[r][c] == false) {
        if (is_reproduction(board, r, c)) {
          printf("set_to_true => %d, %d = %d\n", r, c, board->grid[r][c]);
          board->grid[r][c] = true;
        }
      }

      if (board->grid[r][c] == true) {
        if (is_underpopulated(board, r, c) || is_overpopulated(board, r, c)) {
          printf("set_to_false => %d, %d = %d\n", r, c, board->grid[r][c]);
          board->grid[r][c] = false;
        }

        if (is_new_generation_possible(board, r, c)) {
          board->grid[r][c] = true;
        }
      }
    }
  }
};

void draw_state(Board *board) {
  for (int r = 0; r < board->rows; r++) {
    for (int c = 0; c < board->cols; c++) {
      if (board->grid[r][c] == true) {
        int scale = 10;
        DrawRectangle(r * scale, c * scale, scale, scale, WHITE);
      }
    }
  }
};

int main() {
  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Conway's Game of Life");
  SetTargetFPS(1);

  int grid_width = SCREEN_WIDTH / 10;
  int grid_height = SCREEN_HEIGHT / 10;
  // int grid_width = 10;
  // int grid_height = 10;

  Board board = {0};

  board.grid = create_grid(grid_height, grid_width);
  board.rows = grid_height;
  board.cols = grid_width;
  fill_grid(&board);
  // print_grid(&board);
  draw_state(&board);

  while (!WindowShouldClose()) {
    BeginDrawing();
    {
      ClearBackground(DARKGRAY);
      update_state(&board);
      draw_state(&board);
    }
    EndDrawing();
  }

  return 0;
}
