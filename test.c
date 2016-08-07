#include <stdio.h>
#include "gamepad.h"

int main()
{
    void* ctx;

    /* initialize gamepad */
    ctx = init_gamepad();
    if (!ctx) {
        puts("init failed");
        return -1;
    }

    /* terminate gamepad */
    term_gamepad(ctx);
    return 0;
}
