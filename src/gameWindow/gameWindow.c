#include "header.h"


GameWindow *create_game_window() {

    GameWindow *game_window = (GameWindow*)malloc(sizeof(GameWindow));
    if (game_window == NULL) {
        fprintf(stderr, "ERROR : Creation of GameWindow failed");
        return NULL;
    }

    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;

    window = SDL_CreateWindow(GAME_WINDOW_NAME,
                              GAME_WINDOW_BASE_X, GAME_WINDOW_BASE_Y,
                              GAME_WINDOW_BASE_WIDTH, GAME_WINDOW_BASE_HEIGHT,
                              GAME_WINDOW_FLAGS);

    if(window == NULL) {
        fprintf(stderr, "ERROR : SDL_CreateWindow failed : %s", SDL_GetError());
        return NULL;
    }

    SDL_Surface* icon = SDL_LoadBMP("./data/img/logo.bmp");

    if (icon != NULL) {
        SDL_SetWindowIcon(window, icon);
        SDL_FreeSurface(icon);
    } else {
        printf("Erreur de chargement de l'icône : %s\n", SDL_GetError());
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    if (!renderer) {
        fprintf(stderr, "ERROR : SDL_CreateRenderer failed : %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        return NULL;
    }

    SDL_RenderSetLogicalSize(renderer, GAME_WIDTH, GAME_HEIGHT);

    game_window->posX = GAME_WINDOW_BASE_X;
    game_window->posY = GAME_WINDOW_BASE_Y;
    game_window->width = GAME_WINDOW_BASE_WIDTH;
    game_window->height = GAME_WINDOW_BASE_HEIGHT;
    game_window->fullscreen = 0;
    game_window->window = window;
    game_window->renderer = renderer;
    game_window->pending_restore = 0;
    game_window->cameraX = 0;
    game_window->cameraY = 0;
    game_window->zoom = 0.5f;

    return game_window;
}


void free_game_window(GameWindow *game_window) {
    if (game_window != NULL) {
        if (game_window->renderer != NULL) SDL_DestroyRenderer(game_window->renderer);
        if (game_window->window != NULL) SDL_DestroyWindow(game_window->window);
        free(game_window);
    }
}