#include "TitleScene.h"
#include "../../../Utility/InputManager.h"


TitleScene::TitleScene()
{
	//初期化
    MENU_COUNT = 3;
    cursorIndex = 0;
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
	DrawString(0, 0, "Title", GetColor(255, 255, 255));

    const char* menu[] = { "Start", "Option", "Exit" };

    for (int i = 0; i < MENU_COUNT; i++)
    {
        if (i == cursorIndex)
        {
            DrawFormatString(100, 100 + i * 40, GetColor(255, 255, 0), "→ %s", menu[i]);
        }
        else
        {
            DrawFormatString(120, 100 + i * 40, GetColor(255, 255, 255), "%s", menu[i]);
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

    // 決定ボタン（Aボタンなど）
    if (input->GetButtonDown(XINPUT_BUTTON_A))
    {
        switch (cursorIndex)
        {
        case 0:
            // START
            return eSceneType::GAME_MAIN;
            break;
        case 1:
        //    // HELP
        //    ChangeSceneToOption();
        //    break;
        case 2:
            // END
            DxLib_End();
            break;
        }
    }
}
