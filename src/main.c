#include "SDL.h"
#include "SDL_image.h"

static SDL_Window *win;
static SDL_Renderer *ren;

static int setup(void);
static void shutdown(void);
static SDL_bool running = SDL_FALSE;

int main(int argc, char *argv[])
{
    if (setup() < 0)
        shutdown();

    int frames = 0;
    Uint32 last_frame_time = 0;
    char fps[512];

    SDL_Texture *tex = IMG_LoadTexture(ren, "img.png");
    if (tex == NULL) {
        SDL_Log("Error loading texture: %s\n", SDL_GetError());
        shutdown();
        return -1;
    }

    while (running) {
        Uint32 now = SDL_GetTicks64();
        if (now - last_frame_time >= 1000) {
            SDL_snprintf(fps, sizeof(fps), "fps: %i", frames);
            last_frame_time = now;
            frames = 0;
        }
        frames++;

        SDL_Event evt;
        while (SDL_PollEvent(&evt)) {
            if (evt.type == SDL_QUIT)
                running = SDL_FALSE;
        }
        SDL_RenderClear(ren);
        SDL_Rect dstrect = {0, 0, 32, 32};
        for (int l = 0; l < 6; l++) {
            for (int x = 0; x < 1920; x += 32) {
                for (int y = 0; y < 1080; y += 32) {
                    dstrect.x = x;
                    dstrect.y = y;
                    SDL_RenderCopy(ren, tex, NULL, &dstrect);
                }
            }
        }
        SDL_RenderPresent(ren);
    }

    SDL_DestroyTexture(tex);
    shutdown();

    return 0;
}

static int setup(void)
{
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        SDL_Log("SDL_Init Error: %s\n", SDL_GetError());
        return -1;
    }

    win =
        SDL_CreateWindow("ISGT", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                         1920, 1080, SDL_WINDOW_RESIZABLE);
    if (win == NULL) {
        SDL_Log("SDL_CreateWindow Error: %s\n", SDL_GetError());
        return -1;
    }

    ren = SDL_CreateRenderer(win, -1, 0);
    if (ren == NULL) {
        SDL_Log("SDL_CreateRenderer Error: %s\n", SDL_GetError());
        return -1;
    }

    if (IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG) {
        SDL_Log("SDL_image could not initialize! SDL_image Error: %s\n",
                IMG_GetError());
        return -1;
    }

    running = SDL_TRUE;

    return 0;
}

static void shutdown(void)
{
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    IMG_Quit();
    SDL_Quit();
}
