#ifndef __LEVEL_H__
#define __LEVEL_H__

#include "gfc_hashmap.h"
#include "gf2d_sprite.h"
#include "simple_json.h"
#include "level_select.h"
#include "ent.h"
#include "camera.h"

typedef struct {

    int t1_points;
    int t2_points;
    int max_points;
    int win_count;
    Uint8 win;
    Uint8 mode;
    Uint8 options;

} LevelInstance;

typedef struct{
    Sprite *background;
    Sprite *parallaxObject;
    GFC_Vector2D *spawns;
    GFC_HashMap *prop_map;
    LevelInstance game;
    int height,width;
} Level;

Level* level_new();

void level_free();

void level_draw(Level *level);

Level *level_create(GamePreset gp);

void load_defs(Level *level);

Uint8 get_win_state();

void set_level(Level *level, const char *levelname);

char* index_level_name(Uint8 id);

void populate_level_index();

Level *get_curr_level();

Uint8 level_count();

void level_draw(Level *level);

void level_update(Level *level);

void setup_camera(Level *level);


#endif
