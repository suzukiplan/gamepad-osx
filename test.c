#include <stdio.h>
#include "gamepad.h"

int main()
{
    void* ctx;

    /* initialize gamepad */
    ctx = gamepad_init();
    if (!ctx) {
        puts("init failed");
        return -1;
    }

    /* terminate gamepad */
    gamepad_term(ctx);
    return 0;
}
