#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        fprintf(stderr, "SDL_Init Error: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }

    SDL_Window *win =
        SDL_CreateWindow("Hello World!", 100, 100, 400, 400, SDL_WINDOW_SHOWN);
    if (win == NULL) {
        fprintf(stderr, "SDL_CreateWindow Error: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }

    SDL_Renderer *ren = SDL_CreateRenderer(
        win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
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

    SDL_Surface *loaded_surface = IMG_Load("img.png");
    if (loaded_surface == NULL) {
        fprintf(stderr, "PNG Error: %s\n", SDL_GetError());
        SDL_DestroyRenderer(ren);
        SDL_DestroyWindow(win);
        SDL_Quit();
        return EXIT_FAILURE;
    }

    SDL_Texture *tex = SDL_CreateTextureFromSurface(ren, loaded_surface);
    if (tex == NULL) {
        fprintf(stderr, "SDL_CreateTextureFromSurface Error: %s\n",
                SDL_GetError());
        SDL_FreeSurface(loaded_surface);
        SDL_DestroyRenderer(ren);
        SDL_DestroyWindow(win);
        SDL_Quit();
        return EXIT_FAILURE;
    }
    SDL_FreeSurface(loaded_surface);

    for (int i = 0; i < 20; i++) {
        SDL_RenderClear(ren);
        SDL_RenderCopy(ren, tex, NULL, NULL);
        SDL_RenderPresent(ren);
        SDL_Delay(100);
    }

    SDL_DestroyTexture(tex);
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();

    return EXIT_SUCCESS;
}
