#ifndef __NPC_H__
#define __NPC_H__

#include "ent.h"
#include "player.h"

Ent *npc_new();
void npc_think(Ent *self);
void npc_update(Ent *self, float deltatime);
void npc_free(Ent *self);
void npc_respawn(Ent *self);

#endif
