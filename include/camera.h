#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "gfc_shape.h"
#include "ent.h"


typedef struct{
    GFC_Vector2D pos;
    GFC_Vector2D size;
    int activeTarget; // -1 - World Origin, 0 - Player, 1+ Entity
    GFC_Rect bounds;

} Camera;

GFC_Vector2D camera_get_pos();

void set_mouse_state(int x, int y, Uint8 m);

GFC_Vector2D get_mouse_pos();

Uint8 click_status();

void camera_set_pos(GFC_Vector2D pos);

void camera_set_bounds(GFC_Rect bounds);

void camera_set_size(GFC_Vector2D size);

void camera_bounding();

void camera_update();

void camera_set_target(int id);

#endif
