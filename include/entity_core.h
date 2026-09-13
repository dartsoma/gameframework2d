#define __ENTITY_CORE_H__
#ifndef __ENTITY_CORE_H__

#include "gfc_vector.h"
#include "gfc_shape.h"

typedef uint32_t EntityID;
typedef uint32_t ComponentMask;

typedef struct{
    EntityID id; // Unique identifier (who is it)
    ComponentMask tag; // Entity tag (what is it)
} Entity;

typedef struct{
    Entity *entity_list;

    uint32_t next_id;

    TransformComponent transform_list;

    KinematicsComponent kinmatics_list;

    ResourceComponent resource_list;

    CollisionComponent collision_list;

    RenderComponent render_list;



} EntityManager;

typedef enum {
EntityTransform = 1, //
EntityKinematics = 2, // Manages physics like direction and acceleration
EntityResource = 4, // Manages health, ammo etc etc
EntityCollision = 8, // Raycasting hitboxes
EntityRender = 16, //
} EntityComponentType;


typedef struct {

    GFC_Vector3D pos;
    GFC_Vector3D rot;
    GFC_Vector3D scale;

} TransformComponent;


typedef struct {

    GFC_Vector3D vel;
    GFC_Vector3D acc;
    GFC_Vector3D direction;
    GFC_Vector3D gravity;

} KinematicsComponent;

typedef struct {
// some kind of sprite function

} RenderComponent;

typedef struct {

    float max;
    float min;

} EntityResource;

typedef struct {
    EntityResource *data;
} ResourceComponent;

typedef struct {

    uint32_t layer;
    uint32_t mask;


} CollisionComponent;


void entity_manager_init();

void entity_manager_close();

void entity_free();

Entity *entity_create();

Uint8 is_entity_valid():

entity_manager_draw(uint32_t id);

entity_draw(uint32_t id);



#endif
