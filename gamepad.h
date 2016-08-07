#ifdef __cplusplus
extern "C" {
#endif

void* gamepad_init(int useGamePad, int useKeybord, int useMouse);
void gamepad_set_callback(void* ctx, void (*callback)(int type, int page, int usage, int value));
void gamepad_term(void* ctx);

#ifdef __cplusplus
};
#endif
