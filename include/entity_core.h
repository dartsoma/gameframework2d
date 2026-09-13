#define __ENTITY_CORE_H__
#ifndef __ENTITY_CORE_H__

#include "gfc_shape.h"
#include "gfc_vector.h"

typedef uint32_t EntityID;
typedef uint32_t ComponentMask;

typedef struct {
    EntityID id; // Unique identifier (who is it)
    ComponentMask tag; // Entity tag (what is it)
    void (*think)(struct Entity* self); // Called every frame
    void (*update)(struct Entity* self); // Called on deltatime
    void (*free)(struct Entity* self);
} Entity;

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
    // function varies

} RenderComponent;

typedef struct {

    float max;
    float min;
    float value;

} EntityResource;

typedef struct {
    EntityResource* data;
} ResourceComponent;

typedef struct {

    uint32_t layer;
    uint32_t mask;

} CollisionComponent;

typedef struct
{

    Entity* entity_list;

    GFC_HashMap entity_hash; // Converts id to i in graph
    CF_Queue open_indices; // Free slots for placing entities

    uint32_t next_id;
    uint32_t entity_count;

    TransformComponent* transform_list;

    KinematicsComponent* kinematics_list;

    ResourceComponent* resource_list;

    CollisionComponent* collision_list;

    RenderComponent* render_list;

} EntityManager;

void entity_manager_init();

void entity_manager_close();

void entity_free();

Entity* entity_create();

Uint8 entity_is_valid();

entity_manager_draw(uint32_t max);

entity_draw(uint32_t id);

void* entity_add_component(uint32_t id, EntityComponentType type, void* data);

void entity_remove_component(uint32_t id, EntityComponentType type, void* data);

void* entity_get_component(uint32_t id, EntityComponentType type, void* data);

#endif
