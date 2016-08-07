#include <unistd.h>
#include <stdio.h>
#include "gamepad.h"

static void callback(int type, int page, int usage, int value)
{
    printf("type=%d, page=%d, usage=%d, value=%d\n", type, page, usage, value);
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
    puts("term");
    gamepad_term(ctx);
    return 0;
}
