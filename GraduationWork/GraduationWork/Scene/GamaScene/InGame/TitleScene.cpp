#include "TitleScene.h"
#include "../../../Utility/InputManager.h"


TitleScene::TitleScene()
{
	//������
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

    return eSceneType::TITLE;
}

void TitleScene::Draw()
{
	DrawString(0, 0, "Title", GetColor(255, 255, 255));
}

void TitleScene::Title_Input()
{

    InputManager* input = InputManager::GetInstance();
    // ���L�[��1����
    if (input->GetButtonDown(XINPUT_BUTTON_DPAD_DOWN))
    {
        cursorIndex++;
        if (cursorIndex >= MENU_COUNT) cursorIndex = 0; // ���[�v������
    }

    // ���L�[��1���
    if (input->GetButtonDown(XINPUT_BUTTON_DPAD_UP))
    {
        cursorIndex--;
        if (cursorIndex < 0) cursorIndex = MENU_COUNT - 1;
    }

    // ����{�^���iA�{�^���Ȃǁj
    if (input->GetButtonDown(XINPUT_BUTTON_A))
    {
        switch (cursorIndex)
        {
        case 0:
            // Start
            eSceneType::GAME_MAIN;
            break;
        //case 1:
        //    // Option
        //    ChangeSceneToOption();
        //    break;
        //case 2:
        //    // END
        //    ExitGame();
        //    break;
        }
    }
}
