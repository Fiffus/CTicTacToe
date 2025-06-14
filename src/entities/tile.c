#include "../../include/components/tile.h"
#include "../../include/utils/params.h"
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_ttf.h>

Tile *tile_create(const int x, const int y, const RGB color, TTF_Font *font, SDL_Renderer *renderer) {
    Tile *tile = (Tile*)malloc(sizeof(Tile));

    tile->rect = (SDL_Rect){
        .x = x,
        .y = y,
        .w = TILE_SIZE,
        .h = TILE_SIZE,
    };

    tile->color = color;

    tile->player_mark = 0;

    tile->font = font;
    tile->renderer = renderer;

    return tile;
}

void tile_update(Tile *tile, const char current_player) {
    if (!tile->player_mark) {
        tile->player_mark = current_player;
    }
}

void tile_render(Tile *tile) {
    SDL_SetRenderDrawColor(
        tile->renderer,
        tile->color.r,
        tile->color.g,
        tile->color.b,
        255
    );
    SDL_RenderFillRect(tile->renderer, &tile->rect);

    if (tile->player_mark) {
        char stringified_mark[2] = {tile->player_mark, '\0'};

        SDL_Color color;
        if (tile->player_mark == 'x') {
            color = (SDL_Color){230, 170, 0, 255};
        } else {
            color = (SDL_Color){0, 170, 230, 255};
        }
        SDL_Surface *player_mark_surface = TTF_RenderText_Blended(
            tile->font, 
            stringified_mark, 
            color
        );
        SDL_Texture *player_mark_texture = SDL_CreateTextureFromSurface(tile->renderer, player_mark_surface);

        int text_w = 30;
        int text_h = 42;

        SDL_Rect text_rect = {
            .x = tile->rect.x + (tile->rect.w - text_w - 3) / 2,
            .y = tile->rect.y + (tile->rect.h - text_h - 6) / 2,
            .w = text_w,
            .h = text_h
        };

        SDL_RenderCopy(
            tile->renderer, 
            player_mark_texture, 
            0, 
            &text_rect
        );

        SDL_DestroyTexture(player_mark_texture);
        SDL_FreeSurface(player_mark_surface);
    }
}

void tile_free(Tile *tile) {
    free(tile);
}