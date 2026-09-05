#define __UI_CORE_H__
#ifndef __UI_CORE_H__

#include "gfc_vector.h"
#include "gfc_shape.h"
#include "gfc_hashmap.h"

/*

 The plan is to have a running count of all total ui elements
 allotting an id to each upon initialzation and incrementing up to 32 bit limit
 whilst keeping the count for future

 */



#define MAX_STACK 8 // Amount of windows active at once
#define MAX_WINDOWS 64
#define MAX_UIELEMENTS 1024


// Use with fixed array method for lower complexity


// object identifier
typedef uint32_t UIObject;

// place in element array
typedef uint32_t UIIndex;


typedef struct UIElement {

    struct UIElement *parent;

    char name[50];
    UIIndex index;
    UIObject id;

    Uint8 visible;

    // component bit mask
    Uint8 comp_mask;

} UIElement;

typedef enum
{
    UI_COMP_TRANSFORM = 1, // Position, Scale, Rotation // should be a default
    UI_COMP_TEXT = 2, // Anything involving text
    UI_COMP_TRIGGER = 4, // click, hover, scroll etc
    UI_COMP_SPRITE = 8, // image, clips, animation
    UI_COMP_CONTAINER = 16, // Contains Children

} UIComponentType;

typedef enum {
  ANCHOR_NONE = 1,
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


// UI COMPONENTS CAN BE STORED IN HASHMAPS and accessed via uielement id


typedef struct
{
    GFC_Vector2D pos;
    GFC_Vector2D offset; // anchor offset
    GFC_Vector2D scale;
    UIAnchorType anchor;
} UICompTransform;


// figure out event system before fiddling
typedef struct
{
    GFC_Shape bounding_shape;
    Uint8 state; // 0 - NONE, 1 - LClick, 2 - RClick,  4 - ScrollUP, 8 - ScrollDOWN, 16 - Hover, 32 - Timed (always triggering)
    Uint32_t framestep;
    int callback[6]; // stores the id of events to be callbackedz
} UICompTrigger;

typedef struct
{
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

  uint32_t size; // how many
  UIElement **root;

} UICompContainer;


typedef struct
{

  Sprite *sprite;
  uint32_t frames;

} UICompSprite;


typedef struct {

    Uint8 loaded;
    UIElement *root;
    char name[50];

} UIWindow;

typedef struct {

  UIElement ui;
  Uint8 active;

} CustomCursor;

typedef struct  {

    UIWindow *active_window[MAX_STACK];
    int draw_order[MAX_STACK];
    GFC_HashMap components[4];
    GFC_HashMap ui;
    CustomCursor cursor; // is there a custom cursor, if not don't account for it

} UIManager;

 void ui_manager_init(UIManager *manager);

 void ui_manager_close(UIManager *manager);

 void ui_window_draw();

 void ui_element_draw();

 void ui_window_update();

 void ui_element_update();

 UIWindow *ui_window_new();

 UIElement *ui_element_new();

 void *ui_window_free();

 void *ui_element_free(); // and the related components too

#endif
