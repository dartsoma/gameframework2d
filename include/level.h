#ifndef __LEVEL_H__
#define __LEVEL_H__

#include "gfc_hashmap.h"
#include "gf2d_sprite.h"
#include "simple_json.h"
#include "camera.h"

typedef struct{
    Sprite *background;
    Sprite *parallaxObject;
    GFC_HashMap *prop_map;
    int height,width;
}Level;

Level* level_new();

void level_free(Level *free);

void level_draw(Level *level);

Level *level_create(const char *levelname);

void load_defs(Level *level);

void set_level(Level *level, const char *levelname);

void level_draw(Level *level);

void setup_camera(Level *level);


#endif
