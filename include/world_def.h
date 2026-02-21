#ifndef __WORLD_DEF_H__
#define __WORLD_DEF_H__

#include "prop.h"


typedef struct {

    int position[2];
    Uint8 scale[2];
    Uint8 color[4];
    char imagePath[50];
    int framewidth;
    int frameheight;
    int framesperline;
    int frames

} Drawdef;

typedef struct
{
    char* name;
    Drawdef draw;
    Uint8* stats[4];

}  Propdef;

typedef struct{

   Propdef props[100];
   int height, width;

} Leveldef;


define_props (){


}

#endif
