#include <IOKit/hid/IOHIDManager.h>

#ifdef __cplusplus
extern "C" {
#endif

void* gamepad_init();
void gamepad_term(void* ctx);

#ifdef __cplusplus
};
#endif
