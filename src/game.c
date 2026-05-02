#include <SDL.h>

#include "simple_logger.h"

#include "rgbstep.h"

#include "font.h"

#include "camera.h"

#include "gfc_input.h"

#include "gf2d_graphics.h"

#include "gf2d_sprite.h"

#include "window.h"

#include "ent.h"

#include "level_editor.h"

#include "player.h"

#include "npc.h"

#include "prop.h"

#include "level.h"



#define GS_EXIT 0
#define GS_MAINMENU 1
#define GS_MAPSELECT 2
#define GS_SHOP 3
#define GS_GAME 4
#define GS_GAMEEND 5
#define GS_EDITOR 6

int main(int argc, char * argv[]) {

    /*variable declarations*/
    Uint8 gamestate = 1;
    Ent * player;
    Level * level;
    char points[256] = "";

    // deltatime
    int lastUpdate = SDL_GetTicks();
    int current;
    float deltastep, deltatime;

    int mx, my;
    float mf = 0;
    Sprite * mouse;
    GFC_Vector2D mCenter = gfc_vector2d(0, 0);

    rgbVal mRgbVal = {
        255,
        0,
        0,
        255
    };
    GFC_Color mouseGFC_Color = gfc_color8(mRgbVal.red, mRgbVal.green, mRgbVal.blue, 255);

    /*program initializtion*/
    init_logger("gf2d.log", 0);
    slog("---==== BEGIN ====---");
    gf2d_graphics_initialize(
        "gf2d",
        1200,
        720,
        1200,
        720,
        gfc_vector4d(0, 0, 0, 255),
                             0);
    gf2d_graphics_set_frame_delay(16);
    gf2d_sprite_init(1024);
    ent_manager_init(1024);

    level = level_create("test");
    link_level(level);
    setup_camera(level);
    player = player_new();

    gfc_input_init("./gfc/sample_config/input.cfg");
    SDL_ShowCursor(SDL_DISABLE);
    mouse = gf2d_sprite_load_all("images/pointer.png", 32, 32, 16, 0);
    slog("press [escape] to quit");
    /*main game loop*/
    font_init();
    load_editor();
    define_windows();
    toggle_windows("mainmenu");
    current = SDL_GetTicks();

    while (gamestate != GS_EXIT) {
        gfc_input_update();

        if(is_quit_pressed() == 1){
            gamestate = GS_EXIT;
        }

        if (SDL_GetMouseState( & mx, & my) & is_mouse_pressed()) {
            set_mouse_state(mx, my, 1);
        } else {
            set_mouse_state(mx, my, 0);
        }
        /*update things here*/


        gf2d_graphics_clear_screen();

        switch (gamestate) {

            case GS_EDITOR:{

                W_Element *el = get_window("editor");
                W_Window *win = (W_Window *) el->data;

                if (win->sprite){
                gf2d_sprite_draw_image(win->sprite, gfc_vector2d(0,0));
                }
                editor_think();
                editor_draw_all();

                if (gfc_input_key_pressed("ESCAPE")) {
                    slog("back from editor");
                    toggle_windows("editor");
                    toggle_windows("mainmenu");
                    gamestate = GS_MAINMENU;
                }


                break;
            }
            case GS_MAINMENU:{

                W_Element *el = get_window("mainmenu");

                W_Window *win = (W_Window *) el->data;

                if (win->sprite){
                    gf2d_sprite_draw_image(win->sprite, gfc_vector2d(0,0));
                }

                if (gfc_input_key_pressed("ESCAPE")) gamestate = GS_EXIT;

                break;

            }

            case GS_GAME: {

                player = get_player();

                ent_think_all();
                camera_update(0);

                level_draw(level);

                rainbowStep( & mRgbVal, 0, 255);
                mouseGFC_Color = gfc_color8(mRgbVal.red, mRgbVal.green, mRgbVal.blue, 255);
                ent_manager_draw_all();

                // UI elements last

                font_draw_test(points, FS_large, GFC_COLOR_BLACK, gfc_vector2d(0, 0));

                if (level -> game.win == 1) {
                    gamestate = GS_MAINMENU;
                }
                break;
            }
        }
        current = SDL_GetTicks();

        deltatime = (current - lastUpdate) / 1000.0f;

        if (deltatime <= 0 || deltatime > 0.1f) {
            deltatime = 0.016f; // Cap at 60 FPS
        }

        deltastep += deltatime;

        update_text(deltatime);

        while (deltastep >= 0.016) {
            if (gamestate == GS_GAME) {
                ent_update_all(0.016);
                strcpy(points, display_points(player) ? display_points(player) : "");
            }

            deltastep -= 0.016;
        }

        lastUpdate = current;
        update_windows( &gamestate);

        mf += 0.1;
        if (mf >= 16.0) mf = 0;

        gf2d_sprite_draw(
            mouse,
            gfc_vector2d(mx, my),
                         NULL, &
                         mCenter,
                         NULL,
                         NULL, &
                         mouseGFC_Color,
                         (int) mf);
        // GFC_Vector2D c = get_mouse_pos();
        // slog("Mouse (%f,%f) center, Player (%f,%f) center", c.x, c.y, player->transform.position.x, player->transform.position.y);
        gf2d_graphics_next_frame(); // render current draw frame and skip to the next frame

        // slog("Rendering at %f FPS",gf2d_graphics_get_frames_per_second());
    }
    clean_ui();
    font_close();
    ent_clear();
    ent_manager_close();
    level_free(level);
    slog("---==== END ====---");
    return 0;
}
/*eol@eof*/
