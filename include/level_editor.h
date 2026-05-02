#ifndef __LEVEL_EDITOR_H__
#define __LEVEL_EDITOR_H__

#include "gfc_vector.h"
#include "camera.h"
#include "world_def.h"

typedef struct {

    char name[25];
    GFC_Vector2D position;
    GFC_Vector2D scale;
    GFC_Color color;
    Sprite* sprite;
    int framewidth;
    int frameheight;
    int framesperline;

} InstancedProp;


typedef struct {

    char name[255];
    char imagepath[255];
    int width, height;

} LevelBase;

typedef struct {

 GFC_Vector2D old_pos;
 GFC_Vector2D old_scale;
 Uint8 modes; // 1 - moving, 2 - scaling, 4 - x, 8 - y

} EditorTransform;


    void load_editor();

    void spawn_prop(GFC_Vector2D vector);

    void editor_think();

    void save_level(LevelBase *base);

    void prop_cycle_l();

    void prop_cycle_r();

    void editor_draw_all();

    char* get_def_name();

    void free_defs();

    void delete_prop();

    void delete_all();


#endif
