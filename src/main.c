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

enum status { PAUSED, PLAYING };

typedef struct {
  enum status status;
  bool displayHelp;
} State;

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
      board->grid[r][c] = GetRandomValue(0, 30) > 27 ? true : false;
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
  bool **new_grid = create_grid(board->rows, board->cols);
  for (int r = 0; r < board->rows; r++) {
    for (int c = 0; c < board->cols; c++) {
      if (board->grid[r][c] == false) {
        if (is_reproduction(board, r, c)) {
          new_grid[r][c] = true;
        }
      }

      if (board->grid[r][c] == true) {
        if (is_underpopulated(board, r, c) || is_overpopulated(board, r, c)) {
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

void update_cell(Board *board, int row, int col) {
  board->grid[row][col] = !board->grid[row][col];
}

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

void handle_key_presses(Board *board, State *game_state) {
  if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
    Vector2 mousePosition = GetMousePosition();
    update_cell(board, mousePosition.y / 10, mousePosition.x / 10);
  }

  if (IsKeyPressed(KEY_H)) {
    game_state->displayHelp = !game_state->displayHelp;
  }

  if (IsKeyPressed(KEY_SPACE))
    game_state->status = game_state->status == PAUSED ? PLAYING : PAUSED;
}

void draw_help_ui(State *game_state) {
  if (game_state->displayHelp) {
    DrawRectangle(10, 10, 250, 113, Fade(SKYBLUE, 0.95f));
    DrawRectangleLines(10, 10, 250, 113, BLUE);

    DrawText("Keymaps:", 20, 20, 10, BLACK);
    DrawText("h - Toggle help menu", 40, 40, 10, BLACK);
    DrawText("Space - Play / Pause", 40, 60, 10, BLACK);
    DrawText("Guide:", 20, 80, 10, BLACK);
    DrawText("- Pause and then click to update cell", 40, 100, 10, DARKGRAY);
  }

  DrawText(game_state->status == PAUSED ? "PAUSED" : "PLAYING",
           SCREEN_WIDTH / 2 - 10, 5, 20, YELLOW);

  DrawText("press (h) to display help", SCREEN_WIDTH / 2 - 10,
           SCREEN_HEIGHT - 16, 10, SKYBLUE);
}

int main() {
  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Conway's Game of Life");
  SetTargetFPS(60);

  Board board = {0};
  board.rows = SCREEN_HEIGHT / 10;
  board.cols = SCREEN_WIDTH / 10;
  board.grid = create_grid(board.rows, board.cols);

  State game_state = {.status = PLAYING, .displayHelp = false};
  init_grid(&board);

  while (!WindowShouldClose()) {
    BeginDrawing();
    {
      ClearBackground(DARKGRAY);
      draw_help_ui(&game_state);

      handle_key_presses(&board, &game_state);

      draw_state(&board);

      if (game_state.status == PLAYING)
        update_state(&board);
    }
    EndDrawing();
  }

  free_grid(&board);

  return 0;
}
