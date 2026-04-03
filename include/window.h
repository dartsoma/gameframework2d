
/**
 @note each window will have its own blueprint in json
 **/

#define EL_NONE 0
#define EL_BUTTON 1
#define EL_LABEL 2
#define EL_TEXTAREA 3
#define EL_CONTAINER 4

typedef struct {
    GFC_Vector2D w_size;
    GFC_Vector2D w_position;
    Uint8 id;
    W_Element *objs;
    Uint8 active;
} W_Window;

typedef struct {
    void *data;
    void *parent;
    Uint8 type;
    Uint8 zindex;
} W_Element;

typedef struct {
    GFC_Vector2D pos;
    GFC_Vector2D size;
    Sprite *sprite;
    Uint8 EventId;
    uint8 active;
} W_Button;

typedef struct {
    GFC_Vector2D pos;
    GFC_Vector2D size;
    Sprite* background;
    char* obj;
    uint8 active;
} W_Label;

typedef struct {
    GFC_Vector2D pos;
    GFC_Vector2D size;
    char* text;
    uint8 active;
} W_TextArea;

void activate_window(Window *w);

void update_windows(Window *w);

W_Button *create_button(int id);


W_TextArea *create_textarea(int id);


W_Label *create_button(int id);


}
