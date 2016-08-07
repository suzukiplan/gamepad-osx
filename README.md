# gamepad-osx
- mac OS (osx) 用のプログラムで IOKit (HID) を用いて gamepad / joystick を使えるようにするライブラリです
- GameController framework と違い, 様々な種類の ゲームパッド を認識できます

## License
[MIT](https://github.com/suzukiplan/gamepad-osx/blob/master/LICENSE.txt)

## How to use
- 使いたい XCode プロジェクト で以下を実施
  - `gamepad.c` と `gamepad.h` を組み込む
  - IOKit.framework を追加
- APIを利用 (言語インタフェース は C規約です)

## API specification
### initialize
```c
void* gamepad_init();
```
gamepad の device context を返す (失敗時は `NULL` を返す)

### set callback
```c
void gamepad_set_callback(void* ctx, void (*callback)(int type, int page, int usage, int value));
```
gamepad の入力コールバックを設定

### terminate
```c
void gamepad_term(void* ctx);
```
`gamepad_init` で確保した gamepad の device context を開放
