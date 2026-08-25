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

// UPDATE TO MATCH PROPER ECS DEFINITION
typedef UIEntity{

};

typedef enum
{
    UI_COMP_TRANSFORM = 1, // Position, Scale, Rotation // should be a default
    UI_COMP_TEXT = 2, // Anything involving text
    UI_COMP_TRIGGER = 4, // click, hover, scroll etc
    UI_COMP_SPRITE = 8, // image, clips, animation
    UI_COMP_CONTAINER = 16, // Contains Children
    UI_COMP_ANCHOR = 32, // Position relative to its container / window -- REQ: TRANSFORM
    UI_COMP_EVENT = 64 // Call for an external action -- REQ: TRIGGER
    UI_COMP_LAYOUT = 128 // positions children in accordance -- REQ: CONTAINER
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
  UIAnchorType anchor;
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
    char name[50];

    void (* init)(struct UIScreen *);
    void (* destroy)(struct UIScreen *);
    void (* draw_all)(struct UIScreen *);
    void (* update_all)(struct UIScreen*);

} UIScreen;

typedef struct {

  UIElement ui;
  Uint8 active;

} CustomCursor;

typedef struct{

    UIScreen *active_screen[MAX_STACK];
    int draw_order[MAX_STACK];
    GFC_HashMap *cache;
    CustomCursor cursor; // is there a custom cursor, if not don't account for it

} UIManager;


#endif
