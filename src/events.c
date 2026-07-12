#include "header.h"


static void handle_keydown(SDL_Event *event, GameWindow *game_window, SDL_bool *running);
static void toggle_fullscreen(GameWindow *game_window);
static void handle_windowevent(SDL_Event *event, GameWindow *game_window);


static void handle_keydown(SDL_Event *event, GameWindow *game_window, SDL_bool *running) {
    switch (event->key.keysym.sym) {
        case SDLK_ESCAPE:
            printf("Quitting...\n");
            *running = SDL_FALSE;
            break;

        case SDLK_F11:
            toggle_fullscreen(game_window);
            break;

        default:
            break;
    }
}


static void toggle_fullscreen(GameWindow *game_window) {
    if (!game_window->fullscreen) {
        SDL_GetWindowPosition(game_window->window, &game_window->posX, &game_window->posY);
        SDL_GetWindowSize(game_window->window, &game_window->width, &game_window->height);
        printf("Going Fullscreen.");
        SDL_SetWindowFullscreen(game_window->window, SDL_WINDOW_FULLSCREEN_DESKTOP);
        game_window->fullscreen = 1;
    } else {
        SDL_SetWindowFullscreen(game_window->window, 0);
        game_window->fullscreen = 0;
        game_window->pending_restore = 1;
    }
}


static void handle_windowevent(SDL_Event *event, GameWindow *game_window) {
    if (event->window.event == SDL_WINDOWEVENT_RESIZED ||
        event->window.event == SDL_WINDOWEVENT_RESTORED) {
        if (game_window->pending_restore) {
            SDL_SetWindowSize(game_window->window, game_window->width, game_window->height);
            SDL_SetWindowPosition(game_window->window, game_window->posX, game_window->posY);
            game_window->pending_restore = 0;
            printf("Leaving Fullscreen.");
        }
    }
}


void handle_events(GameWindow *game_window, SDL_bool *running) {
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                printf("Quitting...\n");
                *running = SDL_FALSE;
                break;

            case SDL_KEYDOWN:
                handle_keydown(&event, game_window, running);
                break;

            case SDL_WINDOWEVENT:
                handle_windowevent(&event, game_window);
                break;

            default:
                break;
        }
    }
}