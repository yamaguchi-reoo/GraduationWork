#include "GameClear.h"
#include "../../../Utility/InputManager.h"
#include "../../../Utility/SoundManager.h"
#include "../../SceneManager.h"
#include <cmath>

// ===== エフェクト用（cpp内限定） =====
namespace
{
    int frame = 0;
    float clearScale = 0.4f;
    int fadeAlpha = 255;

    int menuAlpha = 0;
    bool isMenuVisible = false;
    bool isInputEnabled = false;

    struct Particle
    {
        float x, y;
        float vy;
    };

    const int PARTICLE_NUM = 50;
    Particle particles[PARTICLE_NUM];


}

GameClear::GameClear()
{
    MENU_COUNT = 2;
    cursorIndex = 0;

    frame = 0;
    clearScale = 0.4f;
    fadeAlpha = 255;
    menuAlpha = 0;
    isMenuVisible = false;
    isInputEnabled = false;

    for (int i = 0; i < PARTICLE_NUM; i++)
    {
        particles[i].x = rand() % 1280;
        particles[i].y = rand() % 720;
        particles[i].vy = 2 + rand() % 4;
    }
    SoundManager::GetInstance()->Play(SoundID::GAME_CLEAR_SE);
    SoundManager::GetInstance()->Play(SoundID::GAME_CLEAR_BGM);
}

GameClear::~GameClear()
{
}

eSceneType GameClear::GetNowSceneType() const
{
    return eSceneType::GAMECLEAR;
}

eSceneType GameClear::Update()
{
    frame++;

    // 画面フェードイン
    if (fadeAlpha > 0)
    {
        fadeAlpha -= 5;
        if (fadeAlpha < 0) fadeAlpha = 0;
    }

    // GAME CLEAR 文字拡大
    if (clearScale < 1.0f)
    {
        clearScale += 0.04f;
        if (clearScale > 1.0f) clearScale = 1.0f;
    }

    // ===== 段階制御 =====
    // ① 最初は文字だけ
    if (frame > 90 && !isMenuVisible)
    {
        isMenuVisible = true;
    }

    // ② メニューフェードイン
    if (isMenuVisible && menuAlpha < 255)
    {
        menuAlpha += 8;
        if (menuAlpha >= 255)
        {
            menuAlpha = 255;
            isInputEnabled = true;
        }
    }

    // パーティクル更新
    for (int i = 0; i < PARTICLE_NUM; i++)
    {
        particles[i].y += particles[i].vy;
        if (particles[i].y > 720)
        {
            particles[i].y = -10;
            particles[i].x = rand() % 1280;
        }
    }

    if (isInputEnabled)
    {
        return GameClear_Select();
    }

    return eSceneType::GAMECLEAR;
}

void GameClear::Draw()
{
    const int SCREEN_W = 1280;
    const int SCREEN_H = 720;

    int centerX = SCREEN_W / 2;
    int centerY = SCREEN_H / 2;

    // ===== 背景暗転 =====
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, 120);
    DrawBox(0, 0, SCREEN_W, SCREEN_H, GetColor(0, 0, 0), TRUE);
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

    // ===== 紙吹雪 =====
    for (int i = 0; i < PARTICLE_NUM; i++)
    {
        DrawBox(
            (int)particles[i].x,
            (int)particles[i].y,
            (int)particles[i].x + 6,
            (int)particles[i].y + 6,
            GetColor(255, 255, 0),
            TRUE
        );
    }

    // ===== GAME CLEAR 文字 =====
    const char* shadowText = "GAME ";
    const char* clearText = "CLEAR";

    double t = frame * 0.08;
    int rainbowColor = GetColor(
        (int)(128 + 127 * std::sin(t)),
        (int)(128 + 127 * std::sin(t + 2.0)),
        (int)(128 + 127 * std::sin(t + 4.0))
    );

    int alpha = 180 + (int)(std::sin(frame * 0.1) * 75);
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);

    //SetFontSize((int)(64 * clearScale));

    int shadowWidth = GetDrawStringWidthToHandle(
        shadowText, strlen(shadowText), SceneManager::titleFont
    );
    int clearWidth = GetDrawStringWidthToHandle(
        clearText, strlen(clearText), SceneManager::titleFont
    );

    int startX = centerX - (shadowWidth + clearWidth) / 2;
    int y = centerY - 200;

    DrawStringToHandle(
        startX,
        y,
        shadowText,
        rainbowColor,
        SceneManager::titleFont
    );

    DrawStringToHandle(
        startX + shadowWidth,
        y,
        clearText,
        rainbowColor,
        SceneManager::titleFont
    );

    //SetFontSize(64);
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

    // ===== メニュー（フェード） =====
    if (isMenuVisible)
    {
        SetDrawBlendMode(DX_BLENDMODE_ALPHA, menuAlpha);

        const char* menu[] = { "TITLE", "EXIT" };

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

        SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
    }

    // ===== フェードイン =====
    if (fadeAlpha > 0)
    {
        SetDrawBlendMode(DX_BLENDMODE_ALPHA, fadeAlpha);
        DrawBox(0, 0, SCREEN_W, SCREEN_H, GetColor(0, 0, 0), TRUE);
        SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
    }
}

eSceneType GameClear::GameClear_Select()
{
    InputManager* input = InputManager::GetInstance();

    if (input->GetButtonDown(XINPUT_BUTTON_DPAD_DOWN))
    {
        cursorIndex = (cursorIndex + 1) % MENU_COUNT;
        SoundManager::GetInstance()->Play(SoundID::CURSOR);
    }

    if (input->GetButtonDown(XINPUT_BUTTON_DPAD_UP))
    {
        cursorIndex--;
        if (cursorIndex < 0) cursorIndex = MENU_COUNT - 1;
        SoundManager::GetInstance()->Play(SoundID::CURSOR);
    }

    if (input->GetButtonDown(XINPUT_BUTTON_A))
    {
        SoundManager::GetInstance()->Stop(SoundID::GAME_CLEAR_BGM);
        SoundManager::GetInstance()->Play(SoundID::GAME_CLEAR_SELECT);
        switch ((GameClear_MENU)cursorIndex)
        {
        case GameClear_MENU::TITLE:
            return eSceneType::TITLE;
        case GameClear_MENU::END:
            return eSceneType::EXIT;
        }

        
    }

    return eSceneType::GAMECLEAR;
}
