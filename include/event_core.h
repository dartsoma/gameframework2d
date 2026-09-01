#define __EVENT_CORE_H__
#ifndef __EVENT_CORE_H__

typedef struct EventData {
    Uint8 type_id;
    size_t data_size; // size of event specific data
    void* data; // event specific date
    void (*free)(void* data);

} EventData;

// primary structure is linked list due to ambigious amount of events

typedef struct Event {
    int id; // event ID
    int frame; // gets the time of initialization in total game frames
    EventData* data; // ambigious data
    struct Event* next; // next in queue for events
} EventData;

typedef void (*EventCallback)(Event* event, void* event_data);

typedef struct EventListener {
    Uint8 type_id;
    EventCallback callback; // function to call
    void* context; // context data for callback
    int priority; // order called, lowest to highest
} EventListener;

// Catalog of types

typedef struct EventTypeRegistry {
    Uint8 type_id;
    char name[64];
    size_t data_size; // expected size of data based on type
    void (*data_free)(void* data); // how to free data type
} EventTypeRegistry;

typedef struct EventQueue {
    Event* head;
    Event* tail;
    int count;
    int max_events;
} EventQueue;

typedef struct EventSystem {
    EventQueue queue;
    GFC_HashMap* registry; // holds event types by type_id
    GFC_HashMap* listeners; // holds event listeners by name
    int next_event_id;
    Uint8 running;
} EventSystem;

void event_system_init(EventSystem* system, uint32_t max_queue_size);

void event_system_shutdown(EventSystem* system);

// register new event
uint32_t event_register_type(EventSystem* system,
    const char* name,
    size_t data_size,
    void (*cleanup)(void* data));

// create a new event (with data)
Event* event_create(EventSystem* system,
    uint32_t event_type_id,
    void* data,
    size_t data_size);

// push event to queue (thread-safe)
bool event_push(EventSystem* system, Event* event);

// process all events in queue
void event_process_queue(EventSystem* system);

// add a listener for an event type
bool event_add_listener(EventSystem* system,
    uint32_t event_type_id,
    EventCallback callback,
    void* user_data,
    int priority);

// remove a listener
bool event_remove_listener(EventSystem* system,
    uint32_t event_type_id,
    EventCallback callback,
    void* user_data);

// create and push event in one call
bool event_emit(EventSystem* system,
    uint32_t event_type_id,
    void* data,
    size_t data_size);

// event type ID frome name
Uint8 event_get_type_id(EventSystem* system, const char* name);

// check if event type exists
bool event_type_exists(EventSystem* system, Uint8 type_id);

#endif
