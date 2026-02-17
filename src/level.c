
#include "simple_logger.h"

#include "level.h"

Level* level_new(){
    Level *level;
    level = gfc_allocate_array(sizeof(Level),1);
    if(!level)return NULL;
    return level;
}

void level_free(Level *level){
    if(!level)return;
    gf2d_sprite_free(level->background);
    memset(level, 0, sizeof(Level));
    free(level);
}

Level *level_create(const char *background){

    Level *level;
    level = level_new();
    level->width = 1200;
    level->height = 720;
    if(!level) {
        slog("no level init");
        return NULL;
    }
    if(background){
        level->background = gf2d_sprite_load_image(background);
    }

    return level;
}

/*
    Level *load_props(const char *filepath)
    {
        const char *str;
        Level *level
        SJson *json;
        SJson *config;
        if (!filepath)return NULL;
        json = sj_load(filepath);
        if (!json)  return NULL;

        config = sj_object_get_value(json, "whatiwant");

    }
*/
void level_draw(Level *level){

    GFC_Vector2D position, offset;
    position = gfc_vector2d(0,0);
    gfc_vector2d_sub(offset, position, camera_get_pos());
    if(!level){
        return;
    }
    if(level->background){

    gf2d_sprite_draw_image(level->background, offset);
    } else {

        slog("no background");
    }
}


void setup_camera(Level *level){
    camera_set_target(0); // Default Player
    camera_set_pos(gfc_vector2d(0,0));
    camera_set_bounds(gfc_rect(0,0, 5000, 5000));
    camera_set_size(gfc_vector2d(level->width, level->height));
    camera_bounding();

}

