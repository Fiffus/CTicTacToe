#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_main.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include "../include/components/tile.h"
#include "../include/components/board.h"

#define BOARD_SIZE 30

int main() {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        printf("SDL_Init failed: %s\n", SDL_GetError());
        return 1;
    }

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"); 

    if (TTF_Init() == -1) {
        printf("TTF_Init failed: %s\n", TTF_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow(
        "TicTacToe", 
        SDL_WINDOWPOS_CENTERED, 
        SDL_WINDOWPOS_CENTERED, 
        BOARD_SIZE*TILE_SIZE,
        BOARD_SIZE*TILE_SIZE, 
        SDL_WINDOW_SHOWN
    );

    if (!window) {
        TTF_Quit();
        SDL_Quit();
        return  1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    if (!renderer) {
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    TTF_Font *font = TTF_OpenFont("res/font/inter.ttf", 128);
    if (!font) {
        printf("Loading ttf file failed\n");
        return 1;
    }

    Board *board = board_create(BOARD_SIZE, BOARD_SIZE, font, renderer);    

    SDL_Event event;

    int quit = 0;
    int clicked = 0;
    char player_x = 'x';
    char player_o = 'o';
    char current_player = 'x';
    char winner = 0;

    while (!quit) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = 1;
            }

            if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    quit = 1;
                }
                if (event.key.keysym.sym == SDLK_SPACE) {
                    current_player = player_x;
                    winner = 0;
                    board_reset(board);
                }
            }

            clicked = event.type == SDL_MOUSEBUTTONDOWN;
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        if (clicked && !winner) {
            board_update(board, current_player);
            if (current_player == player_x) {
                current_player = player_o;
            } else {
                current_player = player_x;
            }
            winner = board_check_winner(board);
            clicked = 0;
        }

        board_render(board);

        SDL_Surface *win_message_surface;
        SDL_Texture *win_message_texture;

        if (winner) {
            char stringified_text[14];
            snprintf(stringified_text, sizeof(stringified_text), "Player %c won!", winner);

            win_message_surface = TTF_RenderText_Blended(font, stringified_text, (SDL_Color){230, 230, 230, 255});
            win_message_texture = SDL_CreateTextureFromSurface(renderer, win_message_surface);

            int width = win_message_surface->w/2;
            int height = win_message_surface->h/2;

            SDL_Rect text_rect = {
                .x = (BOARD_SIZE*TILE_SIZE)/2-width/2, 
                .y = (BOARD_SIZE*TILE_SIZE)/2-height/2,
                .w = width,
                .h = height
            };

            SDL_RenderCopy(
                renderer, 
                win_message_texture, 
                0, 
                &text_rect
            );
        }

        SDL_RenderPresent(renderer);

        SDL_DestroyTexture(win_message_texture);
        SDL_FreeSurface(win_message_surface);
    }

    board_free(board);
    TTF_CloseFont(font);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    TTF_Quit();
    SDL_Quit();

    return 0;
}