#ifndef __ENT_H__
#define __ENT_H__

// The entity class



#include <SDL.h>
#include "gfc_text.h"
#include "gfc_shape.h"
#include "gf2d_sprite.h"

typedef enum {

    CM_NONE = -1,
    CM_WORLD = 0,
    CM_TEAM1 = 1,
    CM_TEAM2 = 2,
    CM_FFA = 3,
    CM_ITEM = 4,
    CM_HAZARD = 5

} CollisionMask;

typedef struct {

    GFC_Vector2D position;
    GFC_Vector2D scale;
    GFC_Vector2D center;
    GFC_Vector2D velocity;
    float rotation;

} eTransform;

typedef struct {

    GFC_Rect c_box;
    GFC_Vector2D c_dim;
    CollisionMask c_mask;
    GFC_Color c_color;
} eCollide;

typedef struct {


    int oc_all;
    int oc_world;
    int oc_dynamic;

// category
    int oc_sprop;
    int oc_dprop;
    int oc_item;
    int oc_npc;
    int oc_player;

} eCOUNT;

typedef struct Entity {

    Uint8 _inuse; // no touch
    Uint8 _tags; // no touch
    GFC_TextLine name;
    eTransform transform;
    eCollide collide;
    Sprite *sprite;
    GFC_Color color;
    float frame;
    int* stats;
    void (*think) (struct Entity *self);    // called every frame
    void (*update) (struct Entity *self);   // called every frame
} Ent;


#define TAG_STATIC 0
#define TAG_DYNAMIC 1
#define TAG_CONSUMABLE 2
#define TAG_PLAYER 3
#define TAG_NPC 4
#define TAG_PARTICLE 5


typedef struct {

    Ent *entList;
    Uint32 entMax;
    Sprite *hitbox;

} EntManager;

/**
 * @brief frees all entities before freeing the manager
 ***/

void ent_manager_draw_all();

void ent_collide_all();

void ent_collide(Ent* self);

void ent_manager_think_all();

void ent_manager_close();

/**
 * @brief Instantiates the global manager variable
 ***/

void ent_manager_init(Uint32 max);


/**
 * @brief get a pointer to free entity
 * @return NULL if out of entitties, a pointer to a blank entity otherwise
 ***/

Ent *ent_new();
Ent *ent_new_rev();

/**
 * @brief frees and entity
 * @param self the entity to free
 * @note don't use the memory address after this call'
 ***/


void ent_free(Ent* self);

void ent_clear();

void ent_think_all();

void ent_update_all();

void insert_collision_layer(Ent* self);

Ent *index_ent(int id);

#endif
