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
  - Objective-c の XCode プロジェクトで利用できます (実証済み)
  - swift の XCode プロジェクトでも多分利用できます (未確認)

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
- gamepad の入力コールバックを設定
- type, page, usage, value の値を参照することで入力状態を確認できる

_参考: ELECOM の JC-U3312S シリーズでテストした結果_

|type|page|usage|value|input|description|
|:---:|:---:|:---:|:---:|:---|:---|
|1|1|48|0|`X axis -`|左カーソルを入力|
|1|1|48|255|`X axis +`|右カーソルを入力|
|1|1|48|128|`X axis 0`|左右カーソルを離す|
|1|1|49|0|`Y axis -`|上カーソルを入力|
|1|1|49|255|`Y axis +`|下カーソルを入力|
|1|1|49|128|`Y axis 0`|上下カーソルを離す|
|2|9|1~10|0 or 1|`button #usage`|ボタン1〜10を 押す(value=1) or 離す(value=0)|

### terminate
```c
void gamepad_term(void* ctx);
```
`gamepad_init` で確保した gamepad の device context を開放
