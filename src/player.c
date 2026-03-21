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

if(gfc_input_key_down(" ")){
    move(self, JUMP);
}



}

void move(Ent *self, int dir){
    if(!self)return;

    double direction = 0;

    if (dir == LEFT){
        direction += -1;
    }

    if (dir == RIGHT){
       direction += 1;
    }

    direction *= self->stats[SPEED];

    if (fabs(self->transform.velocity.x) < self->stats[SPEED]) {

       self->transform.velocity.x = direction;

    }

    if (dir == JUMP && ((self->status & 3) == 1)) {
        self->status |= 2;
         self->transform.velocity.y -= self->stats[JUMPPOWER];
         slog("jump");
    }

}


void player_update(Ent *self, float deltatime)
{
    if(!self)return;

    // check if airborne

    if ((self->status & 1) == 0){

        self->transform.velocity.y += 800.0f    *deltatime;

    } else if ((self->status & 3) == 1){

        self->transform.velocity.y = 0;

    }
    if ((self->status & 2 ) == 2) {
        if(self->transform.velocity.y >= 0)
        {
            self->status -= 2;
        }
    }

    self->transform.position.x += self->transform.velocity.x * deltatime;
    self->transform.position.y += self->transform.velocity.y * deltatime;

    // all gravity bound objects must fall


    if(fabs(self->transform.velocity.x) < GFC_EPSILON ){
        self->transform.velocity.x = 0;
    } else if ((self->status >> 2 & 4) == 0){
        // not walking so declerate
    self->transform.velocity.x *= 0.1;
    }

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
    self->stats = (int*) malloc(sizeof(int) * 5);
    self->stats[HEALTH] = 100;
    self->stats[ARMOR] = 0;
    self->stats[SPEED] = 500.0f;
    self->stats[JUMPPOWER] = 300.0f;
    self->_tags = TAG_PLAYER;


    self->frame=0;
    self->color = gfc_color8(255,255,255, 255);
    self->transform.velocity = gfc_vector2d(0,0);
    self->transform.position = gfc_vector2d(0,0);
    self->transform.center = gfc_vector2d(0,0);

    // yes this is a magic number, I'm making it the scale of the player's width and height in pixels
    self->collide.c_dim = gfc_vector2d(12.8,12.8);
    self->collide.c_color = GFC_COLOR_RED;
    self->collide.c_mask = CM_FFA;

    self->think = player_think;
    self->update = player_update;

    insert_collision_layer(self);

    return self;

}

