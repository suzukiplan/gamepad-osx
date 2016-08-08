# gamepad-osx
- mac OS (osx) 用のゲームプログラムで IOKit を用いて HID (gamepad / joystick / keybord / mouse) の入力を検出するライブラリです
- GameController framework と違い, 様々な種類の ゲームパッド を認識できます

## License
[MIT](https://github.com/suzukiplan/gamepad-osx/blob/master/LICENSE.txt)

## How to use
- 使いたい XCode プロジェクト で以下を実施
  - `gamepad.c` と `gamepad.h` を組み込む
  - IOKit.framework を追加
- APIを利用 (言語インタフェース は C規約です)
  - Objective-c の XCode プロジェクトで利用できます (実証済み)
  - swift の XCode プロジェクトでも多分利用できます (未確認)

## Example
```c
#include "gamepad.h"

/* describe callback function */
void callback(int type, int page, int usage, int value)
{
    printf("type=%d, page=%d, usage=%d, value=%d\n", type, page, usage, value);
}

int main()
{
    /* initialize gamepad */
    ctx = gamepad_init(1, 1, 0);
    if (!ctx) {
        puts("init failed");
        return -1;
    }

    /* set callback */
    gamepad_set_callback(ctx, callback);

    ~~~ main procedure ~~~

    /* terminate gamepad */
    gamepad_term(ctx);
    return 0;
}
```

## API specification
### initialize gamepad
```c
void* gamepad_init(int useGamePad, int useKeybord, int useMouse)
```
- gamepad の device context を返す (失敗時は `NULL` を返す)
- `useGamePad` : ゲームパッド/ジョイステック を 使用する=1 or 使用しない=0
- `useKeybord` : キーボード を 使用する=1 or 使用しない=0
- `useMouse` : マウス/トラックパッド を 使用する=1 or 使用しない=0

### set callback
```c
void gamepad_set_callback(void* ctx, void (*callback)(int type, int page, int usage, int value));
```
- gamepad, keybord, mouse の入力コールバックを設定
- type, page, usage, value の値を参照することで入力状態を確認できる

#### 参考: ELECOM の JC-U3312S シリーズでテストした結果
device name = `Elecom Wired Gamepad`

|type|page|usage|value|input|description|
|:---:|:---:|:---:|:---:|:---:|:---|
|1|1|48|0|`X axis -`|左カーソルを入力|
|1|1|48|255|`X axis +`|右カーソルを入力|
|1|1|48|128|`X axis 0`|左右カーソルを離す|
|1|1|49|0|`Y axis -`|上カーソルを入力|
|1|1|49|255|`Y axis +`|下カーソルを入力|
|1|1|49|128|`Y axis 0`|上下カーソルを離す|
|2|9|1~10|0 or 1|`button #usage`|ボタン1〜10を 押す(value=1) or 離す(value=0)|

#### 参考: MacBook Pro の キーボード でテストした結果
device name = `Apple Internal Keyboard / Trackpad`

|type|page|usage|value|input|description|
|:---:|:---:|:---:|:---:|:---:|:---|
|2|7|80|0 or 1|←|左カーソルを 押す(value=1) or 離す(value=0)|
|2|7|79|0 or 1|→|右カーソルを 押す(value=1) or 離す(value=0)|
|2|7|82|0 or 1|↑|上カーソルを 押す(value=1) or 離す(value=0)|
|2|7|81|0 or 1|↓|下カーソルを 押す(value=1) or 離す(value=0)|
|2|7|29|0 or 1|Z|Zキーを 押す(value=1) or 離す(value=0)|
|2|7|27|0 or 1|X|Xキーを 押す(value=1) or 離す(value=0)|
|2|7|225|0 or 1|SHIFT|SHIFTキーを 押す(value=1) or 離す(value=0)|
|2|7|41|0 or 1|ESC|ESCキーを 押す(value=1) or 離す(value=0)|

### terminate gamepad
```c
void gamepad_term(void* ctx);
```
`gamepad_init` で確保した gamepad の device context を開放
