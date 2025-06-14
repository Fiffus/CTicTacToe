#pragma once

#include "tile.h"
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_ttf.h>

typedef struct Board {
    int rows, cols;
    Tile **tiles;
} Board;

Board *board_create(const int rows, const int cols, TTF_Font *font, SDL_Renderer *renderer);

void board_reset(Board *board);

void board_mark_winner(Board *board, const int row, const int col, const int threshold, const int direction);

char board_check_winner(Board *board);

void board_update(Board *board, const char current_player);

void board_render(Board *board);

void board_free(Board *board);