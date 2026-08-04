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

#define ZOOM_MIN 0.05f
#define ZOOM_MAX 2.0f
#define ZOOM_WHEEL_STEP 0.1f
#define ZOOM_KEY_SPEED 0.02f

struct GameWindow {
    int posX, posY;
    int width, height;
    int fullscreen;
    SDL_Window *window;
    SDL_Renderer *renderer;
    int pending_restore;
    int cameraX, cameraY;
    float zoom;
};

typedef struct GameWindow GameWindow;

GameWindow *create_game_window();
void free_game_window(GameWindow *game_window);

#endif