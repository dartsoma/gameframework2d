#include "simple_logger.h"
#include "prop.h"


Ent *prop_new(){
    Ent *self;
    self = ent_new_rev();

    if(!self){
    slog("failed to initialized prop");
    }

/*
    self->sprite = gf2d_sprite_load_all(
        "images/Barrel-2.png",
        32,
        32,
        7,
        0
    );



    if(type == 0){
        // barrel
    self->color = gfc_color8(255,255,255,255);

    } else {
        // explosive barrel
    self->color = gfc_color8(255,0,0,255);
    }

    self->transform.velocity = gfc_vector2d(0,0);
    gfc_vector2d_copy(self->transform.position, pos);
    self->transform.scale = gfc_vector2d(3,3);
    */
    self->stats = (int*) malloc(sizeof(int) * 4);

    self->frame=0;
    self->transform.velocity = gfc_vector2d(0,0);
    self->think = prop_think;
    self->update = prop_update;

    return self;

}

void prop_think(Ent *self){
    if (!self)return;

}

void prop_update(Ent *self){
    if (!self)return;
    self->frame += 0.025;
    if (self->frame >= 7) self->frame = 0;

}

