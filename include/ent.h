#ifndef __ENT_H__
#define __ENT_H__

// The entity class



#include <SDL.h>
#include "gfc_text.h"
#include "gf2d_sprite.h"

typedef struct {

    GFC_Vector2D position;
    GFC_Vector2D scale;
    GFC_Vector2D center;
    GFC_Vector2D velocity;
    float rotation;

} eTransform;


typedef struct Entity {

    Uint8 _inuse; // no touch
    Uint8 _tags; // no touch
    GFC_TextLine name;
    eTransform transform;
    Sprite *sprite;
    GFC_Color color;
    float frame;
    int* stats;
    void (*think) (struct Entity *self);// called every frame
    void (*update) (struct Entity *self);// called every frame
} Ent;

typedef struct {
    Ent *entList;
    Uint32 entMax;

} EntManager;

/**
 * @brief frees all entities before freeing the manager
 ***/

void ent_manager_draw_all();

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

Ent *index_ent(int id);

#endif
