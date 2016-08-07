#include <IOKit/hid/IOHIDManager.h>
#include <unistd.h>
#include <stdio.h>
#include "gamepad.h"

static void callback(int type, int page, int usage, int value)
{
    printf("type=%d, page=%d, usage=%d, value=%d\n", type, page, usage, value);

    /* end main loop if push esc key */
    if (2 == type && 7 == page && 41 == usage && 0 == value) {
        CFRunLoopStop(CFRunLoopGetCurrent());
    }
}

int main()
{
    void* ctx;
    char buf[80];
    int i;

    /* initialize gamepad */
    ctx = gamepad_init();
    if (!ctx) {
        puts("init failed");
        return -1;
    }

    /* set callback */
    gamepad_set_callback(ctx, callback);

    CFRunLoopRun();

    /* terminate gamepad */
    gamepad_term(ctx);
    return 0;
}
