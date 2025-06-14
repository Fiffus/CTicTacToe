#include "../../include/components/board.h"
#include "../../include/utils/params.h"
#include "../../include/utils/funcs.h"
#include <SDL2/SDL_mouse.h>
#include <SDL2/SDL_ttf.h>

Board *board_create(const int rows, const int cols, TTF_Font *font, SDL_Renderer *renderer) {
    Board *board = (Board*)malloc(sizeof(Board));
    board->rows = rows;
    board->cols = cols;
    board->tiles = (Tile**)malloc(sizeof(Tile*)*rows*cols);

    RGB black = { .r = 30, .g = 30, .b = 30};
    RGB gray = { .r = 40, .g = 40, .b = 40};

    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < cols; col++) {
            RGB tile_color = black;
            if (row % 2) {
                if (col % 2) {
                    tile_color = gray;
                }
            } else {
                if (!(col % 2)) {
                    tile_color = gray;
                }
            }
            board->tiles[row*cols+col] = tile_create(
                col * TILE_SIZE, 
                row * TILE_SIZE, 
                tile_color,
                font,
                renderer
            );
        }
    }

    return board;
}

void board_reset(Board *board) {
    RGB black = { .r = 30, .g = 30, .b = 30};
    RGB gray = { .r = 40, .g = 40, .b = 40};

    for (int row = 0; row < board->rows; row++) {
        for (int col = 0; col < board->cols; col++) {
            RGB tile_color = black;
            if (row % 2) {
                if (col % 2) {
                    tile_color = gray;
                }
            } else {
                if (!(col % 2)) {
                    tile_color = gray;
                }
            }
            board->tiles[row*board->cols+col]->color = tile_color;
            board->tiles[row*board->cols+col]->player_mark = 0;
        }
    }
}

void board_mark_winner(Board *board, const int row, const int col, const int threshold, const int direction) {
    RGB color = {200, 0, 50};
    switch (direction) {
        case 0:
            for (int i = 0; i < threshold; i++) {
                board->tiles[row*board->cols+col+i]->color = color;
            }
            break;
        case 1:
            for (int i = 0; i < threshold; i++) {
                board->tiles[(row+i)*board->cols+col]->color = color;
            }
            break;
        case 2:
            for (int i = 0; i < threshold; i++) {
                board->tiles[(row+i)*board->cols+col+i]->color = color;
            }
            break;
        case 3:
            for (int i = 0; i < threshold; i++) {
                board->tiles[(row+i)*board->cols+col-i]->color = color;
            }
            break;
    }
}

char board_check_winner(Board *board) {
    for (int row = 0; row < board->rows; row++) {
        for (int col = 0; col < board->cols; col++) {
            if (!board->tiles[row*board->cols+col]->player_mark) continue;

            int horizontal[2] = {0, 0};
            int vertical[2] = {0, 0};
            int tl_br[2] = {0, 0};
            int tr_bl[2] = {0, 0};

            for (int i = 0; i < 5; i++) {
                if (board->tiles[row*board->cols+col+i]->player_mark == 'x') {
                    horizontal[0]++;
                } else if (board->tiles[row*board->cols+col+i]->player_mark == 'o') {
                    horizontal[1]++;
                }
                if (board->tiles[(row+i)*board->cols+col]->player_mark == 'x') {
                    vertical[0]++;
                } else if (board->tiles[(row+i)*board->cols+col]->player_mark == 'o') {
                    vertical[1]++;
                }
                if (board->tiles[(row+i)*board->cols+col+i]->player_mark == 'x') {
                    tl_br[0]++;
                } else if (board->tiles[(row+i)*board->cols+col+i]->player_mark == 'o') {
                    tl_br[1]++;
                }
                if (board->tiles[(row+i)*board->cols+col-i]->player_mark == 'x') {
                    tr_bl[0]++;
                } else if (board->tiles[(row+i)*board->cols+col-i]->player_mark == 'o') {
                    tr_bl[1]++;
                }
            }

            int threshold = 5;
            for (int i = 0; i < 2; i++) {
                if (horizontal[i] == threshold) {
                    board_mark_winner(board, row, col, threshold, 0);
                    return player(i);
                }
                if (vertical[i] == threshold) {
                    board_mark_winner(board, row, col, threshold, 1);
                    return player(i);
                }
                if (tl_br[i] == threshold) {
                    board_mark_winner(board, row, col, threshold, 2);
                    return player(i);
                }
                if (tr_bl[i] == threshold) {
                    board_mark_winner(board, row, col, threshold, 3);
                    return player(i);
                }
            }
        }
    }
    return 0;
}

void board_update(Board *board, const char current_player) {
    int mouse_x;
    int mouse_y;
    
    SDL_GetMouseState(&mouse_x, &mouse_y);

    int tile_row = mouse_y / board->rows;
    int tile_col = mouse_x / board->cols;

    tile_update(board->tiles[tile_row*board->cols+tile_col], current_player);
}

void board_render(Board *board) {
    for (int row = 0; row < board->rows; row++) {
        for (int col = 0; col < board->cols; col++) {
            tile_render(board->tiles[row*board->cols+col]);
        }
    }
}

void board_free(Board *board) {
    for (int row = 0; row < board->rows; row++) {
        for (int col = 0; col < board->cols; col++) {
            tile_free(board->tiles[row*board->cols+col]);
        }
    }
    free(board->tiles);
    free(board);
}