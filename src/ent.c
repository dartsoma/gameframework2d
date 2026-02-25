#include "simple_logger.h"
#include "ent.h"
#include "camera.h"

static EntManager entManager = {0};

void ent_manager_init(Uint32 max) {

    if(!max){
        slog("cannot initialize entity system with zero entities");
        return;
    }
    entManager.entList = gfc_allocate_array(sizeof(Ent), max);
    if(!entManager.entList){
        slog("failed to allocate %i entities", max);
        return;
    }
    entManager.entMax = max;

    atexit(ent_manager_close);
    slog("initialized entity System.");
}

void ent_manager_close(){
int i;
if (!entManager.entList)return;
    for (i = 0; i < entManager.entMax; i++){
        ent_free(&entManager.entList[i]);
    }
    free(entManager.entList);
    memset(&entManager, 0, sizeof(EntManager));
    slog("closed entity system");

}

/**
 * @brief Ent Create / Destroy
 **/

Ent *ent_new(){
    int i;
    if (!entManager.entList){
        slog("eMan not initialized");
        return NULL;
    }
    for (i = 0; i < entManager.entMax; i++){
        if(entManager.entList[i]._inuse) continue;
        entManager.entList[i]._inuse = 1;
        entManager.entList[i].transform.position = gfc_vector2d(0, 0);
        entManager.entList[i].transform.scale = gfc_vector2d(1, 1);
        // set default

        return &entManager.entList[i];
    }
    return NULL;
}

// adds entities to back of the list, for props and misc sprites and such

Ent *ent_new_rev(){
    int i;
    if (!entManager.entList){
        slog("eMan not initialized");
        return NULL;
    }
    for (i = entManager.entMax-1; i > 0; i--){
        if(entManager.entList[i]._inuse) continue;
        entManager.entList[i]._inuse = 1;
        entManager.entList[i].transform.position = gfc_vector2d(0, 0);
        entManager.entList[i].transform.scale = gfc_vector2d(1, 1);
        // set default
        return &entManager.entList[i];
    }
    return NULL;
}


void ent_free(Ent *self){
    if(!self) return;
    if(self->sprite)gf2d_sprite_free(self->sprite);
    memset(self, 0, sizeof(Ent));
}


void ent_clear()    {
    int i;
    if (!entManager.entList){
        slog("Entity Manager not initialized");
        return;
    }
    for (i = 0; i < entManager.entMax; i++){
        if(!(entManager.entList[i]._inuse)) continue;
        ent_free(&entManager.entList[i]);
    }
}
/**
 * @brief Ent Draw Functions
**/

void ent_draw(Ent *self){
    if(!self)return;
    GFC_Vector2D offset, position;
    offset = camera_get_pos();

    gfc_vector2d_sub(position, self->transform.position, offset);

    if(self->sprite){
    gf2d_sprite_render(
        self->sprite,
        position,
        &self->transform.scale,
        &self->transform.center,
        &self->transform.rotation,
        NULL,
        &self->color,
        NULL,
        (Uint32) self->frame);
    }
}

void ent_manager_draw_all(){
    int i;
    if (!entManager.entList){
        slog("Entity Manager not initialized");
        return;
    }
    for (i = 0; i < entManager.entMax; i++){
        if(entManager.entList[i]._inuse == 0) continue;
        ent_draw(&entManager.entList[i]);
    }
}

void ent_update(Ent *self){
    if(!self)return;


    if(self->update)self->update(self);
}


void ent_think(Ent *self){
    if(!self)return;

    if(self->think)self->think(self);
}

void ent_update_all(){
    int i;
    for (i = 0; i < entManager.entMax; i++)  {
        if(!entManager.entList[i]._inuse)continue;
        ent_update(&entManager.entList[i]);
    }
}
void ent_think_all(){
    int i;
    for (i = 0; i < entManager.entMax; i++)  {
        if(!entManager.entList[i]._inuse)continue;
        ent_think(&entManager.entList[i]);
    }
}


Ent *index_ent(int id){

    return &entManager.entList[id];

}

