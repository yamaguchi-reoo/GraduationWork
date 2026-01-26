#include "TitleScene.h"
#include "../../../Utility/InputManager.h"
#include "../../SceneManager.h"


TitleScene::TitleScene()
{
	//初期化
    MENU_COUNT = 3;
    cursorIndex = 0;
    Title_Img = LoadGraph("Resource/Images/Title/Title.png");
}

TitleScene::~TitleScene()
{
}

eSceneType TitleScene::GetNowSceneType() const
{
	return eSceneType::TITLE;
}

eSceneType TitleScene::Update()
{

    return Title_Select();
}

void TitleScene::Draw()
{
    //タイトル画像
    DrawGraph(0, 0, Title_Img, TRUE);

    // 画面全体を少し暗くする
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, 120); // 0?255（数値が大きいほど暗くなる）
    DrawBox(0, 0, 1280, 720, GetColor(0, 0, 0), TRUE);
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

    const int SCREEN_W = 1280;
    const int SCREEN_H = 720;

    int centerX = SCREEN_W / 2;
    int centerY = SCREEN_H / 2;

    // ===== タイトル文字分割 =====
    const char* shadowText = "SHADOW";
    const char* boundText = " UNBOUND";

    int shadowColor = GetColor(120, 120, 120); // 暗め
    int boundColor = GetColor(255, 255, 255); // 明るめ

    // 文字幅取得
    int shadowWidth = GetDrawStringWidthToHandle(
        shadowText,
        strlen(shadowText),
        SceneManager::titleFont
    );

    int boundWidth = GetDrawStringWidthToHandle(
        boundText,
        strlen(boundText),
        SceneManager::titleFont
    );

    int totalWidth = shadowWidth + boundWidth;

    int startX = centerX - totalWidth / 2;
    int y = centerY - 200;

    // ===== 描画 =====
    DrawStringToHandle(
        startX,
        y,
        shadowText,
        shadowColor,
        SceneManager::titleFont
    );

    DrawStringToHandle(
        startX + shadowWidth,
        y,
        boundText,
        boundColor,
        SceneManager::titleFont
    );

    // ===== メニュー =====
    const char* menu[] = { "START", "HELP", "EXIT" };

    for (int i = 0; i < MENU_COUNT; i++)
    {
        int x = centerX - 120;
        int menuY = centerY - 10 + i * 50;

        if (i == cursorIndex)
        {
            DrawFormatStringToHandle(
                x - 40, menuY,
                GetColor(255, 255, 0),
                SceneManager::menuFont,
                "> %s",
                menu[i]
            );
        }
        else
        {
            DrawFormatStringToHandle(
                x, menuY,
                GetColor(255, 255, 255),
                SceneManager::menuFont,
                "%s",
                menu[i]
            );
        }
    }
}



eSceneType TitleScene::Title_Select()
{

    InputManager* input = InputManager::GetInstance();
    // ↓キーで1つ下へ
    if (input->GetButtonDown(XINPUT_BUTTON_DPAD_DOWN))
    {
        cursorIndex++;
        if (cursorIndex >= MENU_COUNT) cursorIndex = 0; // ループさせる
    }

    // ↑キーで1つ上へ
    if (input->GetButtonDown(XINPUT_BUTTON_DPAD_UP))
    {
        cursorIndex--;
        if (cursorIndex < 0) cursorIndex = MENU_COUNT - 1;
    }

    if (input->GetButtonDown(XINPUT_BUTTON_A)) {
        switch (cursorIndex) {
        case 0: return eSceneType::GAMECLEAR;
        case 1: /* HELP 未実装 */ break;
        case 2: return eSceneType::EXIT;   
        }
    }
    return eSceneType::TITLE; // 変更なし
}
