#include "header.h"


static void handle_keydown(SDL_Event *event, GameWindow *game_window, SDL_bool *running);
static void handle_mouse_wheel(SDL_Event *event, GameWindow *game_window);
static void toggle_fullscreen(GameWindow *game_window);
static void handle_windowevent(SDL_Event *event, GameWindow *game_window);
static void apply_zoom(GameWindow *game_window, float delta);


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


static void handle_mouse_wheel(SDL_Event *event, GameWindow *game_window) {
    if (event->wheel.y > 0) {
        apply_zoom(game_window, ZOOM_WHEEL_STEP);
    }
    else if (event->wheel.y < 0) {
        apply_zoom(game_window, -ZOOM_WHEEL_STEP);
    }
}


static void apply_zoom(GameWindow *game_window, float delta) {
    float old_zoom = game_window->zoom;
    float new_zoom = old_zoom + delta;

    if (new_zoom < ZOOM_MIN) new_zoom = ZOOM_MIN;
    if (new_zoom > ZOOM_MAX) new_zoom = ZOOM_MAX;

    if (new_zoom == old_zoom) {
        return;
    }

    /* Centre actuellement affiché, en coordonnées indépendantes du zoom */
    float center_x = (game_window->cameraX + GAME_WIDTH / 2.0f) / old_zoom;
    float center_y = (game_window->cameraY + GAME_HEIGHT / 2.0f) / old_zoom;

    game_window->zoom = new_zoom;

    /* Nouvelle position de caméra pour conserver ce centre à l'écran */
    float new_camera_x = center_x * new_zoom - GAME_WIDTH / 2.0f;
    float new_camera_y = center_y * new_zoom - GAME_HEIGHT / 2.0f;

    int map_width_px = (int)(MAP_WIDTH * TILE_SIZE * new_zoom);
    int map_height_px = (int)(MAP_HEIGHT * TILE_SIZE * new_zoom);

    int max_camera_x = (map_width_px > GAME_WIDTH) ? (map_width_px - GAME_WIDTH) : 0;
    int max_camera_y = (map_height_px > GAME_HEIGHT) ? (map_height_px - GAME_HEIGHT) : 0;

    if (new_camera_x < 0) new_camera_x = 0;
    if (new_camera_y < 0) new_camera_y = 0;
    if (new_camera_x > max_camera_x) new_camera_x = (float)max_camera_x;
    if (new_camera_y > max_camera_y) new_camera_y = (float)max_camera_y;

    game_window->cameraX = (int)new_camera_x;
    game_window->cameraY = (int)new_camera_y;
}


static void toggle_fullscreen(GameWindow *game_window) {
    if (!game_window->fullscreen) {
        SDL_GetWindowPosition(game_window->window, &game_window->posX, &game_window->posY);
        SDL_GetWindowSize(game_window->window, &game_window->width, &game_window->height);
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
            
            case SDL_MOUSEWHEEL:
                handle_mouse_wheel(&event, game_window);
                break;

            default:
                break;
        }
    }
}


void handle_camera_movement(GameWindow *game_window) {
    const Uint8 *keystate = SDL_GetKeyboardState(NULL);

    int map_width_px = (int)(MAP_WIDTH * TILE_SIZE * game_window->zoom);
    int map_height_px = (int)(MAP_HEIGHT * TILE_SIZE * game_window->zoom);

    int max_camera_x = (map_width_px > GAME_WIDTH) ? (map_width_px - GAME_WIDTH) : 0;
    int max_camera_y = (map_height_px > GAME_HEIGHT) ? (map_height_px - GAME_HEIGHT) : 0;

    if (keystate[SDL_SCANCODE_W]) game_window->cameraY -= CAMERA_SPEED;
    if (keystate[SDL_SCANCODE_S]) game_window->cameraY += CAMERA_SPEED;
    if (keystate[SDL_SCANCODE_A]) game_window->cameraX -= CAMERA_SPEED;
    if (keystate[SDL_SCANCODE_D]) game_window->cameraX += CAMERA_SPEED;

    if (game_window->cameraX < 0) game_window->cameraX = 0;
    if (game_window->cameraY < 0) game_window->cameraY = 0;
    if (game_window->cameraX > max_camera_x) game_window->cameraX = max_camera_x;
    if (game_window->cameraY > max_camera_y) game_window->cameraY = max_camera_y;

    if (keystate[SDL_SCANCODE_KP_PLUS] || keystate[SDL_SCANCODE_EQUALS]) {
        apply_zoom(game_window, ZOOM_KEY_SPEED);
    }
    if (keystate[SDL_SCANCODE_KP_MINUS] || keystate[SDL_SCANCODE_MINUS]) {
        apply_zoom(game_window, -ZOOM_KEY_SPEED);
    }
}