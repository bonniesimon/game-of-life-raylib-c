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

void init_grid(Board *board) {
  for (int r = 0; r < board->rows; r++) {
    for (int c = 0; c < board->cols; c++) {
      // r is the y-axis value
      // c is the x-axis value
      board->grid[r][c] = GetRandomValue(0, 9) > 8 ? true : false;
    }
  }
}

void print_grid(Board *board) {
  for (int r = 0; r < board->rows; r++) {
    for (int c = 0; c < board->cols; c++) {
      printf("%d  ", board->grid[r][c]);
    }
    printf("\n");
  }
}

void free_grid(Board *board) {
  for (uint64_t r = 0; r < board->rows; r++) {
    free(board->grid[r]);
  }
  free(board->grid);
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
  // printf("------------------------------\n\n");
  bool **new_grid = create_grid(board->rows, board->cols);
  for (int r = 0; r < board->rows; r++) {
    for (int c = 0; c < board->cols; c++) {
      // printf("%d, %d: %d\n", r, c, board->grid[r][c]);
      if (board->grid[r][c] == false) {
        if (is_reproduction(board, r, c)) {
          // printf("set_to_true => %d, %d = %d\n", r, c, board->grid[r][c]);
          new_grid[r][c] = true;
        }
      }

      if (board->grid[r][c] == true) {
        if (is_underpopulated(board, r, c) || is_overpopulated(board, r, c)) {
          // printf("set_to_false => %d, %d = %d\n", r, c, board->grid[r][c]);
          new_grid[r][c] = false;
        }

        if (is_new_generation_possible(board, r, c)) {
          new_grid[r][c] = true;
        }
      }
    }
  }

  free_grid(board);
  board->grid = new_grid;
};

void draw_state(Board *board) {
  const int scale = 10;

  for (int r = 0; r < board->rows; r++) {
    for (int c = 0; c < board->cols; c++) {
      if (board->grid[r][c] == true) {
        DrawRectangle(c * scale, r * scale, scale, scale, BLACK);
      }
    }
  }
};

int main() {
  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Conway's Game of Life");
  SetTargetFPS(60);

  Board board = {0};
  board.rows = SCREEN_HEIGHT / 10;
  board.cols = SCREEN_WIDTH / 10;
  board.grid = create_grid(board.rows, board.cols);

  init_grid(&board);

  while (!WindowShouldClose()) {
    BeginDrawing();
    {
      ClearBackground(DARKGRAY);

      draw_state(&board);
      update_state(&board);
    }
    EndDrawing();
  }

  free_grid(&board);

  return 0;
}
