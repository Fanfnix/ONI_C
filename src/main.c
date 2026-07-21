#include "header.h"

int main(void) {

    int statut = EXIT_FAILURE;

    /*=== INIT SDL & GAME_WINDOW ===*/
    GameWindow *game_window = NULL;

    int sdlerr = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_EVENTS);
    if (sdlerr < 0) {
        fprintf(stderr, "ERROR : SDL_Init : %s", SDL_GetError());
        goto Quit;
    }

    game_window = create_game_window();
    if (game_window == NULL) goto Quit;

    /*=== INIT GAME ===*/

    srand(time(NULL));

    tags_display_init();
    mass_display_init();
    temperature_display_init();

    elements_init();

    element_texture_init(game_window);

    Map *map = create_map();
    map_init(map);

    SDL_Color blanc = {255, 255, 255, 255};

    SDL_bool running = SDL_TRUE;

    /*=== GAME MAIN LOOP ===*/

    printf("--------------------------------------------------------------------------------\n");

    // elements_show();

    while(running) {

        handle_events(game_window, &running);
        handle_camera_movement(game_window);
        SDL_Delay(20);
        
        SDL_SetRenderDrawColor(game_window->renderer, blanc.r, blanc.g, blanc.b, blanc.a);
        SDL_RenderClear(game_window->renderer);

        map_render(map, game_window->renderer, game_window->cameraX, game_window->cameraY);

        SDL_RenderPresent(game_window->renderer);

    }

    printf("--------------------------------------------------------------------------------\n");

    /*=== GAME CLOSING ===*/

    elements_free();
Quit:
    if (game_window->renderer != NULL) SDL_DestroyRenderer(game_window->renderer);
    if (game_window->window != NULL) SDL_DestroyWindow(game_window->window);
    SDL_Quit();
    return statut;
}