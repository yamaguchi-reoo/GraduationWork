#include "TitleScene.h"
#include "../../../Utility/InputManager.h"
#include "../../../Utility/SoundManager.h"
#include "../../SceneManager.h"


TitleScene::TitleScene()
{
	//初期化
    MENU_COUNT = 3;
    cursorIndex = 0;
    Title_Img = LoadGraph("Resource/Images/Title/Title.png");

    blinkAlpha = 255.0f;
    blinkUp = false;

    isFading = false;
    fadeAlpha = 0.0f;
    nextScene = eSceneType::TITLE;

    fadeDelayTimer = 0;

    //BGM
    SoundManager::GetInstance()->Play(SoundID::TITLE_BGM);
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
    // 点滅処理（既存）
    const float blinkSpeed = 2.0f;
    if (blinkUp)
    {
        blinkAlpha += blinkSpeed;
        if (blinkAlpha >= 255.0f) {
            blinkAlpha = 255.0f;
            blinkUp = false;
        }
    }
    else
    {
        blinkAlpha -= blinkSpeed;
        if (blinkAlpha <= 100.0f) {
            blinkAlpha = 100.0f;
            blinkUp = true;
        }
    }

    // 待機タイマー処理
    if (fadeDelayTimer > 0)
    {
        fadeDelayTimer--;
        return eSceneType::TITLE;
    }

    // フェード処理
    if (nextScene != eSceneType::TITLE)
    {
        isFading = true;
        fadeAlpha += 1.5f; // フェード速度（調整可）
        if (fadeAlpha >= 255.0f)
        {
            fadeAlpha = 255.0f;
            SoundManager::GetInstance()->Stop(SoundID::TITLE_BGM);
            return nextScene; // 完全に暗くなったら遷移
        }
    }

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
            SetDrawBlendMode(DX_BLENDMODE_ALPHA, (int)blinkAlpha);
            DrawFormatStringToHandle(
                x - 40, menuY,
                GetColor(255, 255, 0),
                SceneManager::menuFont,
                "> %s",
                menu[i]
            );
            SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
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

    if (isFading)
    {
        SetDrawBlendMode(DX_BLENDMODE_ALPHA, (int)fadeAlpha);
        DrawBox(0, 0, 1280, 720, GetColor(0, 0, 0), TRUE);
        SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
    }

}



eSceneType TitleScene::Title_Select()
{
    InputManager* input = InputManager::GetInstance();

    // フェード中 or 待機中は入力を無視
    if (isFading || fadeDelayTimer > 0) {
        return eSceneType::TITLE;
    }

    if (input->GetButtonDown(XINPUT_BUTTON_DPAD_DOWN))
    {
        cursorIndex++;
        if (cursorIndex >= MENU_COUNT) cursorIndex = 0;
        SoundManager::GetInstance()->Play(SoundID::CURSOR);
    }

    if (input->GetButtonDown(XINPUT_BUTTON_DPAD_UP))
    {
        cursorIndex--;
        if (cursorIndex < 0) cursorIndex = MENU_COUNT - 1;
        SoundManager::GetInstance()->Play(SoundID::CURSOR);
    }

    if (input->GetButtonDown(XINPUT_BUTTON_A)) {
        switch (cursorIndex) {
        case 0:
            nextScene = eSceneType::GAME_MAIN;
            fadeDelayTimer = FADE_DELAY; // 1秒待つ

            SoundManager::GetInstance()->Play(SoundID::PUSH);
            break;
        case 1:
            // HELP 未実装
            SoundManager::GetInstance()->Play(SoundID::CURSOR);
			nextScene = eSceneType::HELP;
			fadeDelayTimer = FADE_DELAY;
            break;
        case 2:
            SoundManager::GetInstance()->Play(SoundID::CURSOR);
            nextScene = eSceneType::EXIT;
            fadeDelayTimer = FADE_DELAY;
            break;
        }
    }

    return eSceneType::TITLE;
}


