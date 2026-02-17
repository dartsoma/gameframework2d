#ifndef __LEVEL_H__
#define __LEVEL_H__

#include "gf2d_sprite.h"
#include "camera.h"

typedef struct{
    Sprite *background;
    Sprite *parallaxObject;
    int height,width;
    GFC_Vector2D *objects;
}Level;

Level* level_new();

void level_free(Level *free);

void level_draw(Level *level);

Level *level_create(const char *background);

void level_draw(Level *level);

void setup_camera(Level *level);


#endif
