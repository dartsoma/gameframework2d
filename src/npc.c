#include "simple_logger.h"
#include "npc.h"
#include "player.h"
#include "camera.h"


Ent *npc_new(){
    Ent *self = player_new();

    if (!self){
        slog("failed to spawn npc");
        return NULL;
    }

    self->_tags = TAG_NPC;
    self->collide.c_mask = CM_TEAM1;

    self->transform.position.x = 100;
    self->transform.position.y = 100;

    self->think = npc_think;
    self->update = npc_update;

    return self;
}

void npc_update(Ent *self, float deltatime){

    if (!self) return;
    player_update(self, deltatime);
    if (self->stats[RESPAWNTIMER] <= 0){
        npc_respawn(self);
        return;
    }
}

void npc_think(Ent *self){
    if (!self) return;

    player_think(self);
}

void npc_free(Ent *self){

    ent_free(self);

}

void npc_respawn(Ent *self){

    Ent *npc = npc_new();
    PlayerData *pd1 = (PlayerData *) self->misc;
    PlayerData *pd2 = (PlayerData *) npc->misc;
    *pd2 = *pd1;
    npc->collide.c_mask = self->collide.c_mask;
    pd2->guns[0].reserve = pd2->guns[0].maxammo;
    pd2->guns[1].reserve = pd2->guns[1].maxammo;
    pd2->guns[2].reserve = pd2->guns[2].maxammo;
    pd2->guns[0].ammo = pd2->guns[0].apr;
    pd2->guns[1].ammo = pd2->guns[1].apr;
    pd2->guns[2].ammo = pd2->guns[2].apr;

    self->_tags = TAG_NONE;
    self->_inuse = 0;
    ent_free(self);
}
