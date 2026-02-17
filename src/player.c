#include "simple_logger.h"


#include "gfc_types.h"
#include "gfc_input.h"
#include "player.h"

void player_think(Ent *self){

    if(!self){
    return;
    }

if(gfc_input_key_down("a")){
    move(self, LEFT);
}

if(gfc_input_key_down("d")){
    move(self, RIGHT);
}

if(gfc_input_key_down("w")){
    move(self, UP);
}

if(gfc_input_key_down("s")){
    move(self, DOWN);
}

}

void move(Ent *self, int dir){
    if(!self)return;

    switch(dir){
        case LEFT:
            gfc_vector2d_add(self->transform.velocity, self->transform.velocity, gfc_vector2d (-1, 0));
            break;
        case RIGHT:
            gfc_vector2d_add(self->transform.velocity, self->transform.velocity, gfc_vector2d (1, 0));
            break;
        case UP:
            gfc_vector2d_add(self->transform.velocity, self->transform.velocity, gfc_vector2d (0, -1));
            break;
        case DOWN:
            gfc_vector2d_add(self->transform.velocity, self->transform.velocity, gfc_vector2d (0, 1));
            break;
    }

    if (self->transform.velocity.x > 1) {self->transform.velocity.x = 1;} else
    if (self->transform.velocity.x < -1) self->transform.velocity.x = -1;
    if (self->transform.velocity.y > 1) {self->transform.velocity.y = 1;} else
    if (self->transform.velocity.y < -1) self->transform.velocity.y = -1;

    gfc_vector2d_scale(self->transform.velocity, self->transform.velocity, self->stats[SPEED]);
}


void player_update(Ent *self)
{
    if(!self)return;
    self->frame += 0.1;
    if (self->frame >= 16) self->frame = 0;

    gfc_vector2d_add(self->transform.position, self->transform.velocity, self->transform.position);
    if(gfc_vector2d_magnitude(self->transform.velocity) < GFC_EPSILON ){
        self->transform.velocity = gfc_vector2d(0, 0);
    }
    gfc_vector2d_scale(self->transform.velocity, self->transform.velocity, 0.1);
}

Ent *player_new(){

    Ent *self;
    self = ent_new();
    if (!self){
        slog("failed to spawn player ent");
        return NULL;
    }
    self->sprite = gf2d_sprite_load_all(
        "images/ed210.png",
        128,
        128,
        16,
        0
    );
    // Default Stats
    self->stats = (int*) malloc(sizeof(int) * 3);
    self->stats[HEALTH] = 100;
    self->stats[ARMOR] = 0;
    self->stats[SPEED] = 1;

    self->frame=0;
    self->color = gfc_color8(255,255,255, 255);
    self->transform.velocity = gfc_vector2d(0,0);
    self->transform.position = gfc_vector2d(0,0);
    self->think = player_think;
    self->update = player_update;
    return self;

}

