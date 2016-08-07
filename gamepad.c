#include "gamepad.h"

struct gamepad_context {
    IOHIDManagerRef hid_manager;
};

void* init_gamepad()
{
    struct gamepad_context* c;
    c = (struct gamepad_context*)malloc(sizeof(struct gamepad_context));
    if (!c) return NULL;
    memset(c, 0, sizeof(struct gamepad_context));
    c->hid_manager = IOHIDManagerCreate(kCFAllocatorDefault, kIOHIDOptionsTypeNone);
    return c;
}

void term_gamepad(void* ctx)
{
    struct gamepad_context* c = (struct gamepad_context*)ctx;
    if (c->hid_manager) CFRelease(c->hid_manager);
    free(c);
}
