#include "simple_logger.h"
#include "player.h"
#include "level.h"
#include "prop.h"

    Ent* flags[2];

Ent *prop_new(){
    Ent *self;
    self = ent_new_rev();

    if(!self){
    slog("failed to initialized prop");
    return NULL;
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

    // slog("%f %f", self->transform.position.x, self->transform.position.y);

    if(self->_tags == TAG_DYNAMIC) {

        if ((self->status & 1) == 0){

            self->transform.velocity.y += 800.0f * deltatime;

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


        if(fabs(self->transform.velocity.x) < GFC_EPSILON ){
            self->transform.velocity.x = 0;
        } else if ((self->status >> 2 & 4) == 0){
            // not walking so declerate
            self->transform.velocity.x *= 0.1;
        }

    }

}

void set_flags(Uint8 team, Ent* self){

   flags[team-1] = self;
}

void prop_trigger (Ent *trigger, Ent *prop, Level *level) {
    char* n;
    n = (char *) prop->misc;


    if (trigger->_tags != TAG_PLAYER && trigger->_tags != TAG_NPC) return;

    if(!trigger->misc) return;

    PlayerData *pd = (PlayerData *) trigger->misc;
    // slog("%s", n);
    if (strcmp(n, "acid") == 0){

    trigger->stats[0] -= 1;
    } else
    // flags
    if (strcmp(n, "t1flag") == 0 && (trigger->collide.c_mask == CM_TEAM2)){
        if(prop->stats[0] > 0){
        trigger->status|=64;
        prop->stats[0] = 0;
        }

    } else if (strcmp(n, "t2flag") == 0 && (trigger->collide.c_mask == CM_TEAM1)){
        if(prop->stats[0] > 0){
            trigger->status|=64;
            prop->stats[0] = 0;
        }
    } else if (strcmp(n, "t1zone") == 0 && (trigger->collide.c_mask == CM_TEAM1)){
        if ((trigger->status & 64) && (trigger->collide.c_mask == CM_TEAM1)){
            flags[1]->stats[0] = 100;
            ent_score(1, 1);
        }
    } else if (strcmp(n, "t2zone") == 0 && (trigger->collide.c_mask == CM_TEAM2)){
        if ((trigger->status & 64) && (trigger->collide.c_mask == CM_TEAM2)){
            flags[0]->stats[0] = 100;
            ent_score(1, 1);
        }
    } else if (strcmp(n, "hillzone") == 0 ) {

        if(level->game.mode == 0){
            if (pd->hillTimer <= 0){
                pd->points += 1;
                pd->hillTimer = 1.0f;
            }
        }
    }


    // Heal Zone
    else if (strcmp(n, "healzone") == 0){

    if (trigger->stats[0] <= 1000){
        trigger->stats[0] += 1;
    } else {
        trigger->stats[0] = 100;
    }
    } else if(strcmp(n, "healthpack") == 0){

        trigger->stats[0] = 1000;
        ent_free(prop);
    } else if(strcmp(n, "armorpack") == 0){

        trigger->stats[1] = 100;
        ent_free(prop);

    } else if(strcmp(n, "ammocrate") == 0){


        // full ammo restock
        pd->guns[0].reserve = pd->guns[0].maxammo;
        pd->guns[1].reserve = pd->guns[1].maxammo;
        pd->guns[2].reserve = pd->guns[2].maxammo;
        ent_free(prop);
    } else if(strcmp(n, "ultcharge") == 0){

        pd->activeCharge = 100;
        ent_free(prop);
    }



}

