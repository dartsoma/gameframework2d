#include "SDL2/SDL_ttf.h"

#include "simple_logger.h"
#include "gf2d_graphics.h"
#include "font.h"

typedef struct {
    GFC_List *fonts;
}FontManager;


static FontManager font_manager = {0};

// closes font
void font_close(){

    int i, c;
    TTF_Font * font;
    c = gfc_list_get_count(font_manager.fonts);

    for (i = 0; i < c; i++){

        font = gfc_list_get_nth(font_manager.fonts, i);
        if (!font)continue;
        TTF_CloseFont(font);
    }
    gfc_list_delete(font_manager.fonts);
    memset(&font_manager, 0,sizeof(FontManager));
    TTF_Quit();
}


// starts font system and grabs font
void font_init (){

    int i;
    TTF_Font * font;
    if (TTF_Init() != 0){
        slog ("TTF sys error");
        return;
    }
    font_manager.fonts = gfc_list_new();
    for (i=0; i < FS_MAX; i++){
    font = TTF_OpenFont("fonts/Bronco.ttf", 10 + (i * 4));
    if (!font){
    slog("failed to open font fonts/Bronco.ttf");
    continue;
    }
    gfc_list_append(font_manager.fonts, font);

    }
}

void font_draw_test(const char *text, FontStyles style, GFC_Color color, GFC_Vector2D position){

    TTF_Font *font;
    SDL_Surface *surface;
    SDL_Texture *texture;
    SDL_Color fg;
    SDL_Rect rect;

    font = gfc_list_get_nth(font_manager.fonts, style);
    if(!font){
        slog("font not loaded");
        return;
    }
    fg = gfc_color_to_sdl(color);
    surface = TTF_RenderUTF8_Blended_Wrapped(font, text, fg, 0);
    if(!surface){
        slog("error with surface");
        return;
    }
    surface = gf2d_graphics_screen_convert(&surface);
    if(!surface){
        slog("error with surface2");
        return;
    }
    texture = SDL_CreateTextureFromSurface(gf2d_graphics_get_renderer(), surface);
    if(!texture){
        SDL_FreeSurface(surface);
        slog("texure error");
        return;
    }
    rect.x = position.y;
    rect.y = position.x;
    rect.w = surface->w;
    rect.h = surface->h;

    SDL_RenderCopy(gf2d_graphics_get_renderer(),
                   texture,
                   NULL,
                   &rect);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);

}

