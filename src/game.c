#include <SDL.h>
#include "simple_logger.h"
#include "rgbStep.h"

#include "font.h"
#include "camera.h"
#include "gfc_input.h"
#include "gf2d_graphics.h"
#include "gf2d_sprite.h"
#include "ent.h"
#include "player.h"
#include "npc.h"
#include "prop.h"
#include "level.h"


int main(int argc, char * argv[])
{


    /*variable declarations*/
    int done = 0;
    const Uint8 * keys;
    Sprite *sprite;
    Ent *player, *npc;
    Level *level;

    // deltatime
    int lastUpdate = SDL_GetTicks();
    int current;
    float deltastep, deltatime;


    int mx,my;
    float mf = 0;
    Sprite *mouse;
    GFC_Vector2D mCenter = gfc_vector2d(0,0);

    rgbVal mRgbVal = {255, 0, 0, 255};
    GFC_Color mouseGFC_Color = gfc_color8(mRgbVal.red,mRgbVal.green,mRgbVal.blue, 255);

    /*program initializtion*/
    init_logger("gf2d.log",0);
    slog("---==== BEGIN ====---");
    gf2d_graphics_initialize(
        "gf2d",
        1200,
        720,
        1200,
        720,
        gfc_vector4d(0,0,0,255),
        0);
    gf2d_graphics_set_frame_delay(16);
    gf2d_sprite_init(1024);
    ent_manager_init(1024);

    level = level_create("test");
    link_level(level);
    setup_camera(level);
    player = player_new();
    npc = npc_new();

    gfc_input_init("./gfc/sample_config/input.cfg");
    SDL_ShowCursor(SDL_DISABLE);
    /*demo setup*/
    sprite = gf2d_sprite_load_image("images/backgrounds/win.png");
    mouse = gf2d_sprite_load_all("images/pointer.png",32,32,16,0);
    slog("press [escape] to quit");
    /*main game loop*/
    font_init();

    while(!done)
    {
        SDL_PumpEvents();   // update SDL's internal event structures
        keys = SDL_GetKeyboardState(NULL); // get the keyboard state for this frame
        if(SDL_GetMouseState(&mx,&my) & SDL_BUTTON_LMASK) {
            set_mouse_state(mx,my, 1);
        } else {
            set_mouse_state(mx,my, 0);
        }
        /*update things here*/
        gfc_input_update();

        // physics
        current = SDL_GetTicks();

        deltatime = (current - lastUpdate) / 1000.0f;

        if (deltatime <= 0 || deltatime > 0.1f) {
            deltatime = 0.016f; // Cap at 60 FPS
        }

        deltastep += deltatime;
        while (deltastep >= 0.016){
            ent_update_all(deltastep);
            deltastep -= 0.016;
        }

        lastUpdate = current;

        ent_think_all();
        camera_update(player->transform.position);
        mf+=0.1;
        if (mf >= 16.0)mf = 0;
        
        gf2d_graphics_clear_screen();// clears drawing buffers
        // all drawing should happen betweem clear_screen and next_frame
            //backgrounds drawn first
            level_draw(level);

            rainbowStep(&mRgbVal,0,255);
            mouseGFC_Color = gfc_color8(mRgbVal.red,mRgbVal.green,mRgbVal.blue, 255);
            ent_manager_draw_all();
            //UI elements last

            font_draw_test(display_points(player), FS_large, GFC_COLOR_BLACK, gfc_vector2d(0,0));

            if (level->game.win == 1){
                gf2d_sprite_draw_image(sprite,gfc_vector2d(0,0));
            }

            gf2d_sprite_draw(
                mouse,
                gfc_vector2d(mx,my),
                NULL,
                &mCenter,
                NULL,
                NULL,
                &mouseGFC_Color,
                (int)mf);
        // GFC_Vector2D c = get_mouse_pos();
        // slog("Mouse (%f,%f) center, Player (%f,%f) center", c.x, c.y, player->transform.position.x, player->transform.position.y);

        gf2d_graphics_next_frame();// render current draw frame and skip to the next frame
        
        if (keys[SDL_SCANCODE_ESCAPE])done = 1; // exit condition
        // slog("Rendering at %f FPS",gf2d_graphics_get_frames_per_second());
    }
    ent_free(npc);
    font_close();
    ent_clear();
    ent_manager_close();
    level_free(level);
    slog("---==== END ====---");
    return 0;
}
/*eol@eof*/
