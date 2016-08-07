#include <IOKit/hid/IOHIDManager.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gamepad.h"

struct gamepad_device {
    char name[1024];
    IOHIDDeviceRef device;
    struct gamepad_device* next;
};

struct gamepad_context {
    IOHIDManagerRef hid_manager;
    void (*callback)(int type, int page, int usage, int value);
    struct gamepad_device* devices_head;
    struct gamepad_device* devices_tail;
};

static void append_matching_dictionary(CFMutableArrayRef matcher, uint32_t page, uint32_t use)
{
    CFMutableDictionaryRef result = CFDictionaryCreateMutable(kCFAllocatorDefault, 0, &kCFTypeDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks);
    if (!result) return;
    CFNumberRef pageNumber = CFNumberCreate(kCFAllocatorDefault, kCFNumberIntType, &page);
    CFDictionarySetValue(result, CFSTR(kIOHIDDeviceUsagePageKey), pageNumber);
    CFRelease(pageNumber);
    CFNumberRef useNumber = CFNumberCreate(kCFAllocatorDefault, kCFNumberIntType, &use);
    CFDictionarySetValue(result, CFSTR(kIOHIDDeviceUsageKey), useNumber);
    CFRelease(useNumber);
    CFArrayAppendValue(matcher, result);
    CFRelease(result);
}

static void device_input(void* ctx, IOReturn result, void* sender, IOHIDValueRef val)
{
    struct gamepad_context* c = (struct gamepad_context*)ctx;
    IOHIDElementRef element = IOHIDValueGetElement(val);
    int type = IOHIDElementGetType(element);
    int page = IOHIDElementGetUsagePage(element);
    int usage = IOHIDElementGetUsage(element);
    int value = IOHIDValueGetIntegerValue(val);
    if (c->callback) c->callback(type, page, usage, value);
}

static void device_attached(void* ctx, IOReturn result, void* sender, IOHIDDeviceRef device)
{
    struct gamepad_context* c = (struct gamepad_context*)ctx;
    struct gamepad_device* d = (struct gamepad_device*)malloc(sizeof(struct gamepad_device));
    CFStringRef name;

    if (!d) return;
    memset(d, 0, sizeof(struct gamepad_device));
    d->device = device;

    name = IOHIDDeviceGetProperty(device, CFSTR(kIOHIDProductKey));
    if (!name) {
        free(d);
        return;
    }
    CFStringGetCString(name, d->name, 1024, kCFStringEncodingUTF8);

#ifdef DEBUG
    printf("attched device: %s\n", d->name);
#endif

    IOHIDDeviceOpen(device, kIOHIDOptionsTypeNone);
    IOHIDDeviceScheduleWithRunLoop(device, CFRunLoopGetCurrent(), kCFRunLoopCommonModes);
    IOHIDDeviceRegisterInputValueCallback(device, device_input, c);

    if (c->devices_tail) {
        c->devices_tail->next = d;
    } else {
        c->devices_head = d;
        c->devices_tail = c->devices_head;
    }
}

static void device_detached(void* ctx, IOReturn result, void* sender, IOHIDDeviceRef device)
{
    struct gamepad_context* c = (struct gamepad_context*)ctx;
    struct gamepad_device* prev;
    struct gamepad_device* cur;
    struct gamepad_device* next;

    for (prev = NULL, cur = c->devices_head; cur; cur = cur->next) {
        if (cur->device == device) {
            if (NULL == prev) {
                c->devices_head = cur->next;
                if (NULL == c->devices_head) c->devices_tail = NULL;
            } else if (cur == c->devices_tail) {
                c->devices_tail = prev;
                prev->next = NULL;
            } else {
                prev->next = cur->next;
            }
#ifdef DEBUG
            printf("detached device: %s\n", cur->name);
#endif
            IOHIDDeviceClose(cur->device, kIOHIDOptionsTypeNone);
            free(cur);
            return;
        }
        prev = cur;
    }
}

void* gamepad_init()
{
    struct gamepad_context* c;
    CFMutableArrayRef matcher;

    c = (struct gamepad_context*)malloc(sizeof(struct gamepad_context));
    if (!c) return NULL;
    memset(c, 0, sizeof(struct gamepad_context));

    c->hid_manager = IOHIDManagerCreate(kCFAllocatorDefault, kIOHIDOptionsTypeNone);
    if (!c->hid_manager) {
        gamepad_term(c);
        return NULL;
    }

    matcher = CFArrayCreateMutable(kCFAllocatorDefault, 0, &kCFTypeArrayCallBacks);
    if (!matcher) {
        gamepad_term(c);
        return NULL;
    }
    append_matching_dictionary(matcher, kHIDPage_GenericDesktop, kHIDUsage_GD_Joystick);
    append_matching_dictionary(matcher, kHIDPage_GenericDesktop, kHIDUsage_GD_GamePad);
    IOHIDManagerSetDeviceMatchingMultiple(c->hid_manager, matcher);
    CFRelease(matcher);

    IOHIDManagerRegisterDeviceMatchingCallback(c->hid_manager, device_attached, c);
    IOHIDManagerRegisterDeviceRemovalCallback(c->hid_manager, device_detached, c);
    IOHIDManagerScheduleWithRunLoop(c->hid_manager, CFRunLoopGetMain(), kCFRunLoopCommonModes);
    IOHIDManagerOpen(c->hid_manager, kIOHIDOptionsTypeNone);
    return c;
}

void gamepad_set_callback(void* ctx, void (*callback)(int type, int page, int usage, int value))
{
    struct gamepad_context* c = (struct gamepad_context*)ctx;
    c->callback = callback;
}

void gamepad_term(void* ctx)
{
    struct gamepad_context* c = (struct gamepad_context*)ctx;
    if (!c) return;
    if (c->devices_head) {
        struct gamepad_device* work;
        struct gamepad_device* next;
        for (work = c->devices_head; work; work = next) {
            next = work->next;
            IOHIDDeviceClose(work->device, kIOHIDOptionsTypeNone);
            free(work);
        }
    }
    if (c->hid_manager) {
        IOHIDManagerUnscheduleFromRunLoop(c->hid_manager, CFRunLoopGetCurrent(), kCFRunLoopCommonModes);
        IOHIDManagerClose(c->hid_manager, kIOHIDOptionsTypeNone);
        CFRelease(c->hid_manager);
    }
    free(c);
}
