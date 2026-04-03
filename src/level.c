
#include "simple_logger.h"
#include "world_def.h"
#include "projectile.h"
#include "weapon.h"
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
    unload_weapons();
    unload_projectiles();
    memset(level, 0, sizeof(Level));
    free(level);
}

Level *level_create(const char *levelname){
    Level *level;
    level = level_new();
    level->width = 1200;
    level->height = 720;
    level->game.max_points = 0;
    level->game.win = 0;
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
        json = sj_load("./def/props.def");
        if (!json){

            slog("bad propdef");
        }

        // lolololololol

        load_weapons();
        load_projectiles();

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

        slog("finished loading defs");
    }


    void set_level(Level *level, const char* levelname){



        int i, j, c;
        if(!level) return;
        SJson *json;
        json = sj_load("./def/levels.def");
        slog("loading level");
        if (!json) {
            slog("bad level def");
        }
        SJson *config, *leveljson;
        slog("loading level");

        config = sj_object_get_value(json, "levels");
        c = sj_array_get_count(config);


        for(i = 0; i < c; i++){

            leveljson = sj_array_get_nth(config,i);
            if(strcmp(levelname,  sj_object_get_string(leveljson, "name")) != 0) continue;

                // level defaults
            sj_object_get_int(leveljson, "width", &level->width );
            sj_object_get_int(leveljson, "height", &level->height );
            level->background = gf2d_sprite_load_image(sj_object_get_string(leveljson, "imagepath"));
            sj_object_get_uint8(leveljson, "gamemode", &level->game.mode);
            sj_object_get_int(leveljson, "winscore", &level->game.win_count);

            // get spawnpoints


            SJson *spawnarray, *array;
            spawnarray = sj_object_get_value(leveljson, "spawns");
            int spawnCount = sj_array_get_count(spawnarray);
            level->spawns = (GFC_Vector2D *) malloc(sizeof(GFC_Vector2D)*spawnCount);


            instance_props(leveljson, level->prop_map);

            for (j = 0; j < spawnCount; j++ ){

                array = sj_array_get_nth(spawnarray, j);
                array = sj_array_get_nth(spawnarray, 0);
                sj_get_float_value(array, &level->spawns[j].x);

                array = sj_array_get_nth(spawnarray, j);
                array = sj_array_get_nth(spawnarray, 1);
                sj_get_float_value(array, &level->spawns[j].y);
            }
            slog("hello");
            // spawn functions
                break;
        }

        sj_free(json);
       // gfc_hashmap_foreach(level->prop_map, (gfc_work_func*)free_propdef);
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

void level_update(Level *level) {

    switch(level->game.mode){

        case 0:
            // Zone

            if(level->game.max_points >= (level->game.win_count) ) {
                // win

                level->game.win = 1;

            }


            break;

        case 1:
            // flag

            if(level->game.t1_points >= level->game.win_count ) {
                // win team 1
                level->game.win = 1;
            }
            if(level->game.t2_points >= level->game.win_count) {
                // win team 2
                level->game.win = 1;

            }


            break;
        case 2:
            // deathmatch

            if(level->game.max_points >= level->game.win_count ) {
                // win
                level->game.win = 1;
            }

            break;


    }


}


void setup_camera(Level *level){
    camera_set_target(0); // Default Player
    camera_set_pos(gfc_vector2d(0,0));
    camera_set_bounds(gfc_rect(0,0, level->width, level->height-100));
    camera_set_size(gfc_vector2d(1200, 700));
    camera_bounding();

}

