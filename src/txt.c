#include "SDL.h"

#include "txt.h"

#define UNICODE_MAX 1112064
#define CODEPOINT_BUFSIZ 1024
#define ATLAS_SIZE 1024

struct txt_font { // 2MB
    SDL_Texture *atlas;
    Uint8 glyphs_x[UNICODE_MAX];
    Uint8 glyphs_y[UNICODE_MAX];
    Uint8 glyph_w, glyph_h;
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
        if (cache->data[cp[i]] == SDL_FALSE && cp[i] > 0) {
            cache->data[cp[i]] = SDL_TRUE;
            cache->count++;
        }
    }
}

struct txt_codepoint_cache *txt_create_codepoint_cache(void)
{
    struct txt_codepoint_cache *cache =
        SDL_calloc(1, sizeof(struct txt_codepoint_cache));
    return cache;
}

struct txt_font *txt_create_font(struct txt_codepoint_cache *cache,
                                 TTF_Font *ttf, SDL_Renderer *ren)
{
    struct txt_font *font = SDL_malloc(sizeof(struct txt_font));
    if (font == NULL)
        return NULL;

    SDL_Color font_color = {255, 255, 255, 255};

    // create dummy glyph for storing its width and height
    SDL_Surface *dummy_glyph = TTF_RenderGlyph32_Blended(ttf, '?', font_color);
    if (dummy_glyph == NULL) {
        txt_destroy_font(font);
        return NULL;
    }
    font->glyph_w = dummy_glyph->w;
    font->glyph_h = dummy_glyph->h;
    SDL_FreeSurface(dummy_glyph);

    // create atlas surface
    SDL_Surface *atlas_surface =
        SDL_CreateRGBSurface(0, ATLAS_SIZE, ATLAS_SIZE, 32, 0, 0, 0, 0);
    if (atlas_surface == NULL) {
        txt_destroy_font(font);
        return NULL;
    }

    // populate atlas surface with cached glyphs
    SDL_Surface *glyph = NULL;
    int cursor_x = 0, cursor_y = 0;
    SDL_Rect glyph_atlas_rect = {0, 0, font->glyph_w, font->glyph_h};
    for (size_t codepoint = '!'; codepoint < UNICODE_MAX; codepoint++) {
        if (cache->data[codepoint] == SDL_TRUE) {
            glyph = TTF_RenderGlyph32_Blended(ttf, codepoint, font_color);
            if (glyph == NULL) {
                SDL_FreeSurface(atlas_surface);
                txt_destroy_font(font);
                return NULL;
            }

            // if glyphs width or height vary, abort
            if (glyph->w != font->glyph_w || glyph->h != font->glyph_h) {
                SDL_FreeSurface(atlas_surface);
                txt_destroy_font(font);
                SDL_FreeSurface(glyph);
                return NULL;
            }

            glyph_atlas_rect.x = cursor_x * font->glyph_w;
            glyph_atlas_rect.y = cursor_y * font->glyph_h;

            SDL_BlitSurface(glyph, &glyph->clip_rect, atlas_surface,
                            &glyph_atlas_rect);
            font->glyphs_x[codepoint] = cursor_x;
            font->glyphs_y[codepoint] = cursor_y;

            // move cursor and verify if it's out of atlas boundaries
            cursor_x++;
            if (cursor_x * font->glyph_w + font->glyph_w > ATLAS_SIZE) {
                cursor_x = 0;
                cursor_y++;
                if (cursor_y * font->glyph_h + font->glyph_h > ATLAS_SIZE) {
                    SDL_FreeSurface(atlas_surface);
                    txt_destroy_font(font);
                    SDL_FreeSurface(glyph);
                    return NULL;
                }
            }

            SDL_FreeSurface(glyph);
        }
    }

    // create atlas texture from atlas surface
    font->atlas = SDL_CreateTextureFromSurface(ren, atlas_surface);
    SDL_FreeSurface(atlas_surface);
    if (font->atlas == NULL) {
        txt_destroy_font(font);
        return NULL;
    }

    return font;
}

void txt_destroy_font(struct txt_font *font)
{
    if (font != NULL) {
        if (font->atlas != NULL)
            SDL_DestroyTexture(font->atlas);
        SDL_free(font);
    }
}
