#include "simple_logger.h"
#include "camera.h"

static Camera camera = {0};
GFC_Vector2D mouse;
Uint8 mbuttons;

void set_mouse_state(int x, int y, Uint8 m) {

    mouse = gfc_vector2d(x,y);
    mouse.x += camera.pos.x;
    mouse.y += camera.pos.y;
    mbuttons = m;
}

GFC_Vector2D get_mouse_pos() {

    return mouse;
}

Uint8 click_status() {
    return mbuttons;
}


GFC_Vector2D camera_get_pos(){

    return camera.pos;
}

void camera_set_pos(GFC_Vector2D pos){

    gfc_vector2d_copy(camera.pos,pos);
    camera_bounding();
}

void camera_set_size(GFC_Vector2D size){

    gfc_vector2d_copy(camera.size, size);
}


void camera_set_bounds(GFC_Rect bounds){
    gfc_rect_copy(camera.bounds,bounds);
}


void camera_update(GFC_Vector2D target){

    GFC_Vector2D position;
    Ent *player;
    player = get_player();
    switch(camera.activeTarget){
        case -1:
            // The World Origin
            position.x = camera.size.x*0.5;
            position.y = camera.size.y*0.5;
            break;
        case 0:

            /**
             * Player
             * @note Player is no longer always index 0
            */

            if (!player) return;
            position.x = player->transform.position.x - (camera.size.x*0.5);
            position.y = player->transform.position.y - (camera.size.y*0.5);
            break;
        default:
            // Other Ent
            position.x = index_ent(camera.activeTarget)->transform.position.x - (camera.size.x*0.5);
            position.y = index_ent(camera.activeTarget)->transform.position.y - (camera.size.y*0.5);
            break;
    }


    camera_set_pos(position);
}

void camera_set_target(int id){

    camera.activeTarget = id;

}


void camera_bounding(){

if((camera.pos.x + camera.size.x) > (camera.bounds.x + camera.bounds.w)) {

    camera.pos.x  = (camera.bounds.x + camera.bounds.w) - camera.size.x;
}

if((camera.pos.y + camera.size.y) > (camera.bounds.y + camera.bounds.h)) {

    camera.pos.y  = (camera.bounds.y + camera.bounds.h) - camera.size.y;
}

if(camera.pos.x < camera.bounds.x) camera.pos.x = camera.bounds.x;
if(camera.pos.y < camera.bounds.y) camera.pos.y = camera.bounds.y;

}
