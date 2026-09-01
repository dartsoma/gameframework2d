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

// Use with fixed array method for lower complexity
typedef uint32_t UIObject;

typedef enum
{
    UI_COMP_TRANSFORM = 1, // Position, Scale, Rotation // should be a default
    UI_COMP_TEXT = 2, // Anything involving text
    UI_COMP_TRIGGER = 4, // click, hover, scroll etc
    UI_COMP_SPRITE = 8, // image, clips, animation
    UI_COMP_CONTAINER = 16, // Contains Children

} UIComponentType;

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


typedef struct UIElement {

    struct UIElement *parent;
    char name[50];
    UIObject id;

    Uint8 visible;
    Uint8 z_overwrite;
    // component bit mask
    int comp_mask;

} UIElement;

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

    UIObject id;

    GFC_Vector2D pos;
    GFC_Vector2D scale;
    UIAnchorType anchor;

} UICompTransform;


// figure out event system before fiddling
typedef struct
{

    UIObject id;
    GFC_Shape bounding_shape;
    Uint8 trigger_mask; // 1 - LClick, 2 - RClick, 4 - Hover, 8 - ScrollUP, 16 - ScrollDOWN, 32 - Idle (always triggering)
    int callback[6]; // stores the id of events to be callbackedz


} UICompTrigger;

typedef struct
{

  UIObject id;

  GFC_Vector2D pos;
  GFC_Vector2D bounds;
  UIAnchorType alignment;

  char *text;
  char *placeholder;
  GFC_Color color;
  float font_size;

  Uint32_t limit; // character limit

} UICompText;

typedef struct
{

  UIObject id;

} UICompContainer;


typedef struct
{

  UIObject id;

} UICompSprite;


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
