#include <IOKit/hid/IOHIDManager.h>

#ifdef __cplusplus
extern "C" {
#endif

void* init_gamepad();
void term_gamepad(void* ctx);

#ifdef __cplusplus
};
#endif
