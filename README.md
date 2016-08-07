# gamepad-osx
- mac OS (osx) の IOKit を用いた ゲームパッド を使うプログラムです
- GameController framework と違い, 様々な種類の ゲームパッド を認識できます

## How to use
- 使いたい XCode プロジェクト で以下を実施
  - `gamepad.c` と `gamepad.h` を組み込む
  - IOKit.framework を追加
- APIを利用 (gamepad の API の 言語インタフェース は C規約です)

## API specification
### initialize/terminate
```c
void* init_gamepad();
void term_gamepad(void* ctx);
```
- `init_gamepad` は gamepad の device context を返す (失敗時は `NULL` を返す)
- `term_gamepad` は `init_gamepad` で確保した gamepad の device context を開放する

