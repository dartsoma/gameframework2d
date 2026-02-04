#include "simple_logger.h"

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
            self->transform.position.x -= self->stats[SPEED];
            break;
        case RIGHT:
            self->transform.position.x += self->stats[SPEED];
            break;
        case UP:
            self->transform.position.y -= self->stats[SPEED];
            break;
        case DOWN:
            self->transform.position.y += self->stats[SPEED];
            break;
    }

}


void player_update(Ent *self)
{
if(!self)return;

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
    self->transform.position = gfc_vector2d(0,0);
    self->think = player_think;
    self->update = player_update;
    return self;

}

