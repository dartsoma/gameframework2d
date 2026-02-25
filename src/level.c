
#include "simple_logger.h"
#include "world_def.h"
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

Level *level_create(const char *levelname){
    Level *level;
    level = level_new();
    level->width = 1200;
    level->height = 720;
    if(!level) {
        slog("no level init");
        return NULL;
    }

    load_defs(level);

    set_level(level, levelname);

    return level;
}


    void load_defs(Level *level)
    {
        int i, c;
        PropDef *propdef;
        SJson *json; //free
        SJson *config, *propjson;
        json = sj_load("./def/.props");
        if (!json)  return;
        // lolololololol
        config = sj_object_get_value(json, "props");
        c = sj_array_get_count(config);
        level->prop_map = gfc_hashmap_new();


        for(i = 0; i < c; i++){

            propjson = sj_array_get_nth(config,i);
            if (!propjson) continue;
            // Create a propdef object
            propdef = create_propdef(propjson);
            // use name as a key
            gfc_hashmap_insert(level->prop_map, propdef->name, propdef);
        }
        sj_free(json);
    }


    void set_level(Level *level, const char* levelname){

        int i, c;

        if(!level) return;
        SJson *json;
        json = sj_load("./def/.levels");
        if (!json)  return;
        SJson *config, *leveljson;

        config = sj_object_get_value(json, "levels");
        c = sj_array_get_count(config);

        for(i = 0; i < c; i++){

            leveljson = sj_array_get_nth(config,i);
            if(strcmp(levelname,  sj_object_get_string(leveljson, "name")) == 0){

                // level defaults
            sj_object_get_int(leveljson, "width", &level->width );
            sj_object_get_int(leveljson, "height", &level->height );
            level->background = gf2d_sprite_load_image(sj_object_get_string(leveljson, "imagepath"));
                // spawn functions
            instance_props(leveljson, level->prop_map);
                break;
            }
        }

        sj_free(json);
        //gfc_hashmap_foreach(level->prop_map, (gfc_work_func*)free_propdef);
        gfc_hashmap_free(level->prop_map);
    }


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

       // slog("no background");
    }
}


void setup_camera(Level *level){
    camera_set_target(0); // Default Player
    camera_set_pos(gfc_vector2d(0,0));
    camera_set_bounds(gfc_rect(0,0, 2000, 2000));
    camera_set_size(gfc_vector2d(level->width, level->height));
    camera_bounding();

}

