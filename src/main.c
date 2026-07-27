#include "header.h"

int main(void) {

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

    element_texture_init(game_window);
    backwall_texture_init(game_window);

    Map *map = create_map();

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

        map_render(map, game_window);

        SDL_RenderPresent(game_window->renderer);

    }

    printf("--------------------------------------------------------------------------------\n");

    /*=== GAME CLOSING ===*/

    element_texture_destroy();
    backwall_texture_destroy();

Quit:
    if (map != NULL) map_free(map);
    free_game_window(game_window);
    SDL_Quit();
    return 0;
}