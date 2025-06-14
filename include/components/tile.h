#pragma once

#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "../utils/params.h"

#define TILE_SIZE 30

typedef struct Tile {
    SDL_Rect rect;
    struct RGB color;
    
    char player_mark;

    TTF_Font *font;
    SDL_Renderer *renderer;
} Tile;

Tile *tile_create(const int x, const int y, const RGB color, TTF_Font *font, SDL_Renderer *renderer);

void tile_update(Tile *tile, const char current_player);

void tile_render(Tile *tile);

void tile_free(Tile *tile);