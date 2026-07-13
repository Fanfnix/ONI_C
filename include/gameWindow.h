#ifndef GAME_WINDOW
#define GAME_WINDOW

#define GAME_WIDTH 1920
#define GAME_HEIGHT 1080

#define GAME_WINDOW_BASE_X SDL_WINDOWPOS_CENTERED
#define GAME_WINDOW_BASE_Y SDL_WINDOWPOS_CENTERED
#define GAME_WINDOW_BASE_WIDTH 1280
#define GAME_WINDOW_BASE_HEIGHT 720

#define GAME_WINDOW_FLAGS (SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE)
#define GAME_WINDOW_NAME "Oxygen Not Included"

typedef struct {
    int posX, posY;
    int width, height;
    int fullscreen;
    SDL_Window *window;
    SDL_Renderer *renderer;
    int pending_restore;
    int cameraX, cameraY;
} GameWindow;

GameWindow *create_game_window();

#endif