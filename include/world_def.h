#ifndef __WORLD_DEF_H__
#define __WORLD_DEF_H__

#include "simple_json.h"
#include "gfc_hashmap.h"
#include "prop.h"


typedef struct
{
    char name[25];
    int position[2];
    Uint8 scale[2];
    Uint8 color[4];
    char imagepath[50];
    int framewidth;
    int frameheight;
    int framesperline;
    Uint8 stats[4];

}  PropDef;

PropDef *create_propdef(SJson *prop);

void free_propdef(PropDef *def);

void instance_props(SJson *leveldef, GFC_HashMap *prop_map);

#endif
