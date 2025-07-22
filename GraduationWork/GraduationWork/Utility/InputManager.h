#pragma once
#include "DxLib.h"

#define D_KEYCODE_MAX (256)     //キーコードの最大数（256キー）
#define MOUSE_MAX (256)         //マウスボタンの最大数

#define BUTTON_MAX 16           //コントローラーボタンの最大数
#define STICK_X 1               //スティックのX軸
#define STICK_Y 2               //スティックのY軸
#define STICK_TILT_MAX 32767.0f //スティックの最大傾き値（XInput仕様）

//マウスカーソルの座標情報を持つ構造体
struct Cursor
{
    int x; //マウスカーソルのX座標
    int y; //マウスカーソルのY座標
};
class InputManager
{
private:
    //クラスの唯一のインスタンス（シングルトンパターン）
    static InputManager* instance;

    //現在のキー入力状態
    char now_key[D_KEYCODE_MAX];
    //前回のキー入力状態
    char old_key[D_KEYCODE_MAX];

    //現在のマウス入力状態
    char now_mouse[MOUSE_MAX];
    //前回のマウス入力状態
    char old_mouse[MOUSE_MAX];

    //マウスカーソルの座標
    Cursor cursor;

    //現在のコントローラーボタン入力状態
    char now_button[BUTTON_MAX];
    //前回のコントローラーボタン入力状態
    char old_button[BUTTON_MAX];

    //XInputデバイスの入力状態
    XINPUT_STATE input;

private:
    //コンストラクタ（privateにすることで外部からのインスタンス生成を防ぐ）
    InputManager() = default;

    //コピーガード（コピー禁止）
    InputManager(InputManager& v) = delete;
    InputManager& operator = (const InputManager& v) = delete;

public:
    /// @brief デストラクタ（シングルトンなのでデフォルト）
    ~InputManager() = default;

public:
    //InputControl のインスタンス
    static InputManager* GetInstance();

    //インスタンスの削除（シングルトンの破棄処理）
    static void DeleteInstance();

public:
    //入力状態を更新する
    void Update();

    //キーボード入力処理
    //キーが押されている間
    bool GetKey(int key_code) const;

    //キーが押された瞬間
    bool GetKeyDown(int key_code) const;

    //キーが離された瞬間
    bool GetKeyUp(int key_code) const;

    // マウス入力処理
    //マウスボタンが押されている間
    bool GetMouse(int mouse);

    //マウスボタンが押された瞬間
    bool GetMouseDown(int mouse);

    //マウスボタンが離された瞬間
    bool GetMouseUp(int mouse);

    //マウスカーソルの座標を取得
    Cursor GetMouseCursor();

    //コントローラ入力処理
    //コントローラのボタンが押されている間
    bool GetButton(int button) const;

    //コントローラのボタンが押された瞬間
    bool GetButtonDown(int button) const;

    //コントローラのボタンが離された瞬間
    bool GetButtonUp(int button) const;

    //左スティックの傾きを取得
    float LeftStickTilt(short l_stick);

    //右スティックの傾きを取得
    float RightStickTilt(short r_stick);

private:
    //キーコードが有効な範囲にあるかを確認
    static bool CheckKeyCodeRange(int key_code);
};

