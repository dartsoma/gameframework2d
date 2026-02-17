#ifndef __camera_h__
#define __camera_h__

#include "gfc_shape.h"
#include "ent.h"


typedef struct{
    GFC_Vector2D pos;
    GFC_Vector2D size;
    int activeTarget; // -1 - World Origin, 0 - Player, 1+ Entity
    GFC_Rect bounds;

} Camera;

GFC_Vector2D camera_get_pos();

void camera_set_pos(GFC_Vector2D pos);

void camera_set_bounds(GFC_Rect bounds);

void camera_set_size(GFC_Vector2D size);

void camera_bounding();

void camera_update();

void camera_set_target(int id);

#endif
