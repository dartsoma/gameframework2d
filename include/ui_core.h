#define __UI_CORE_H__
#ifndef __UI_CORE_H__

#include "gfc_vector.h"
#include "gfc_shape.h"
#include "gfc_hashmap.h"

// Amount of screens active at once
#define MAX_STACK 10
// Amount of screens to be loaded at a time
#define MAX_SCREENS 64
#define MAX_ELEMENTS 1024

// ID for entity
typedef uint32_t UIEntity;


typedef enum
{
    UI_COMP_TRANSFORM = 1, // Position, Scale, Rotation // should be a default
    UI_COMP_TEXT = 2, // Text
    UI_COMP_TRIGGER = 8, // click, hover, scroll
    UI_COMP_SPRITE = 16, // image, clips, animation
    UI_COMP_CONTAINER = 32, // Contians Children
    UI_COMP_ANCHOR = 64, // Position relative to its container / window -- REQ: TRANSFORM
    UI_COMP_EVENT = 128 // Call a button event -- REQ: TRIGGER
    UI_COMP_LAYOUT = 256 // positions children in accordance -- REQ: CONTAINER
} UIComponentType;

typedef struct UIElement {

    struct UIElement *parent;
    char name[50];
    UIEntity id;

    Uint8 visible;
    Uint8 z_index;
    // component bit mask
    int comp_mask;

} UIElement;


typedef struct
{

    UIEntity id;

    GFC_Vector2D pos;
    GFC_Vector2D scale;


} UICompTransform;


typedef struct
{

  UIEntity id;

} UICompEvent;

typedef enum {
    ANCHOR_TOP_L = 1,
    ANCHOR_TOP = 2,
    ANCHOR_TOP_R = 3,
    ANCHOR_CENTER_L = 4,
    ANCHOR_CENTER = 5,
    ANCHOR_CENTER_R = 6,
    ANCHOR_BOTTOM_L = 7,
    ANCHOR_BOTTOM = 8,
    ANCHOR_BOTTOM_R = 9
} UIAnchorType;

typedef struct
{
  UIEntity id;
  UIAnchorType;


} UICompAnchor;

typedef struct
{

    UIEntity id;
    GFC_Shape bounding_shape;


} UICompTrigger;


typedef struct
{

  UIEntity id;

} UICompContainer;

typedef struct
{

  UIEntity id;
  Sprite *sp;

} UICompSprite;

typedef struct
{
  UIEntity id;

} UICompLayout;


typedef struct {

    Uint8 loaded;
    UIElement *root;
    char name[50];CURSOR

    void (* init)(struct UIScreen *);
    void (* destroy)(struct UIScreen *);
    void (* draw_all)(struct UIScreen *);
    void (* update_all)(struct UIScreen*);

} UIScreen;

typedef struct{

    UIScreen *active_screen[MAX_STACK];
    int draw_order[MAX_STACK];
    GFC_HashMap *cache;

} UIManager;


#endif
