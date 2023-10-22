#include "SDL.h"

#include "txt.h"

#define UNICODE_MAX 1112064
#define CODEPOINT_BUFSIZ 1024
#define ATLAS_SIZE 1024

struct txt_font { // 2MB
    SDL_Texture *atlas;
    Uint8 glyph_x[UNICODE_MAX];
    Uint8 glyph_y[UNICODE_MAX];
};

struct txt_codepoint_cache { // 4MB
    SDL_bool data[UNICODE_MAX];
    size_t count;
};

int txt_get_codepoints(Uint32 *cp, size_t cp_maxlen, const char *str)
{
    // ASCII ONLY FOR NOW
    unsigned char byte1;
    size_t str_len = SDL_strlen(str);
    size_t cp_i = 0;
    for (size_t str_i = 0; str_i < str_len; str_i++) {
        byte1 = str[str_i];
        if (byte1 > 0x7f || cp_i >= cp_maxlen)
            return -1;
        cp[cp_i] = byte1;
        cp_i++;
    }

    return 0;
}

void txt_cache_codepoint(struct txt_codepoint_cache *cache, const char *str)
{
    Uint32 cp[CODEPOINT_BUFSIZ] = {0};
    size_t cp_maxlen = SDL_arraysize(cp);
    txt_get_codepoints(cp, cp_maxlen, str);
    for (size_t i = 0; i < cp_maxlen; i++) {
        if (cache->data[cp[i]] == SDL_FALSE) {
            cache->data[cp[i]] = SDL_TRUE;
            cache->count++;
        }
    }
}

struct txt_codepoint_cache *txt_create_codepoint_cache(void)
{
    return SDL_malloc(sizeof(struct txt_codepoint_cache));
}

void txt_destroy_font(struct txt_font *font)
{
    if (font != NULL) {
        if (font->atlas != NULL)
            SDL_DestroyTexture(font->atlas);
        SDL_free(font);
    }
}
