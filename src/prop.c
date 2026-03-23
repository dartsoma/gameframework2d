#include "simple_logger.h"
#include "prop.h"


Ent *prop_new(){
    Ent *self;
    self = ent_new_rev();

    if(!self){
    slog("failed to initialized prop");
    }

    self->stats = (int*) malloc(sizeof(int) * 5);


    self->frame=0;
    self->transform.velocity = gfc_vector2d(0,0);
    self->think = prop_think;
    self->update = prop_update;

    return self;

}

void prop_think(Ent *self){
    if (!self)return;


}

void prop_update(Ent *self, float deltatime){
    if (!self)return;

    if(self->_tags == TAG_DYNAMIC) {

        if ((self->status & 3) == 0){
            self->transform.velocity.y += deltatime * 4;
        }


        self->transform.position.x += self->transform.velocity.x * deltatime;
        self->transform.position.y += self->transform.velocity.y * deltatime;


        if(fabs(self->transform.velocity.x) < GFC_EPSILON ){
            self->transform.velocity.x = 0;
        } else if ((self->status >> 2 & 4) == 0){
            // not walking so declerate
            self->transform.velocity.x *= 0.1;
        }

    }

}

