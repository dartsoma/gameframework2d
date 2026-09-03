#define __EVENT_CORE_H__
#ifndef __EVENT_CORE_H__

#include "GFC_HashMap.h"

typedef struct EventData {
    Uint8 type_id;
    size_t data_size; // size of event specific data
    void* data; // event specific date
    void (*free)(void* data);

} EventData;

// Primary structure is linked list due to ambigious amount of events

typedef struct Event {
    uint32_t id; // event ID
    uint32_t frame; // gets the time of initialization in total game frames
    EventData* data; // ambigious data
    struct Event* next; // next in queue for events
} Event;

typedef void (*EventCallback)(Event* event, void* context);

typedef struct EventListener {
    Uint8 type_id;
    EventCallback callback; // function to call
    void* context; // context data for callback
    uint32_t priority; // order called, lowest to highest
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
    uint32_t count;
    uint32_t max_events;
} EventQueue;

typedef struct EventSystem {
    EventQueue queue;
    GFC_HashMap* registry; // holds event types by type_id
    GFC_HashMap* listeners; // holds event listeners by name
    uint32_t next_event_id;
    Uint8 running;
} EventSystem;

void event_system_init(EventSystem* system, uint32_t max_queue_size);

void event_system_shutdown(EventSystem* system);

// Register new event
uint32_t event_register_type(EventSystem* system,
    const char* name,
    size_t data_size,
    void (*cleanup)(void* data));

// Create a new event (with data)
Event* event_create(EventSystem* system,
    uint32_t event_type_id,
    void* data,
    size_t data_size,
    uint32_t frame);

// Push event to queue (thread-safe)
// Side note: this project is not multithreaded that may come years later
Uint8 event_push(EventSystem* system, Event* event);

// Process all events in queue
Uint8 event_process_queue(EventSystem* system);

// Add a listener for an event type
Uint8 event_add_listener(EventSystem* system,
    uint32_t event_type_id,
    EventCallback callback,
    void* user_data,
    int priority);

// Remove a listener
Uint8 event_remove_listener(EventSystem* system,
    uint32_t event_type_id,
    EventCallback callback,
    void* user_data);

// Create and push event in one call
Uint8 event_emit(EventSystem* system,
    uint32_t event_type_id,
    void* data,
    size_t data_size);

// Event type ID frome name
Uint8 event_get_type_id(EventSystem* system, const char* name);

// Check if event type exists
Uint8 event_type_exists(EventSystem* system, Uint8 type_id);

#endif
