#ifndef __PROP_H__
#define __PROP_H__

#include "ent.h"


Ent *prop_new();

void prop_think(Ent *self);

void prop_update(Ent *self, float deltatime);

void prop_trigger (Ent *trigger, Ent *prop, Level *level);

void set_flags(Uint8 team, Ent* self);

#endif
