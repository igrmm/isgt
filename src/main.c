#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    int running = 1;

    int frames = 0;
    Uint32 last_frame_time = 0;
    char fps[512];

    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        fprintf(stderr, "SDL_Init Error: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }

    SDL_Window *win = SDL_CreateWindow("Hello World!", SDL_WINDOWPOS_CENTERED,
                                       SDL_WINDOWPOS_CENTERED, 1920, 1080,
                                       SDL_WINDOW_RESIZABLE);
    if (win == NULL) {
        fprintf(stderr, "SDL_CreateWindow Error: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }

    // SDL_Renderer *ren = SDL_CreateRenderer(win, -1,
    // SDL_RENDERER_PRESENTVSYNC);
    SDL_Renderer *ren = SDL_CreateRenderer(win, -1, 0);
    if (ren == NULL) {
        fprintf(stderr, "SDL_CreateRenderer Error: %s\n", SDL_GetError());
        if (win != NULL) {
            SDL_DestroyWindow(win);
        }
        SDL_Quit();
        return EXIT_FAILURE;
    }

    int imgFlags = IMG_INIT_PNG;
    if (!(IMG_Init(imgFlags) & imgFlags)) {
        printf("SDL_image could not initialize! SDL_image Error: %s\n",
               IMG_GetError());
    }

    SDL_Texture *tex = IMG_LoadTexture(ren, "img.png");
    if (tex == NULL) {
        fprintf(stderr, "SDL_CreateTextureFromSurface Error: %s\n",
                SDL_GetError());
        SDL_DestroyRenderer(ren);
        SDL_DestroyWindow(win);
        SDL_Quit();
        return EXIT_FAILURE;
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
                goto cleanup;
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

        SDL_SetWindowTitle(win, fps);
    }

cleanup:
    SDL_DestroyTexture(tex);
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();

    return EXIT_SUCCESS;
}
