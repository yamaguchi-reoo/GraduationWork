#include "GameOverScene.h"
#include "../../../Utility/InputManager.h"
#include "../../../Utility/SoundManager.h"
#include "../../SceneManager.h"
#include <DxLib.h>

GameOverScene::GameOverScene()
{
	MENU_COUNT = 3;
	cursorIndex = 0;
	wait_timer = 60;   // 1秒待つ（60fps想定）
}

GameOverScene::~GameOverScene()
{
}

void GameOverScene::Initialize()
{
	SoundManager::GetInstance()->Play(SoundID::GAME_OVER_BGM);
}

eSceneType GameOverScene::GetNowSceneType() const
{
	return eSceneType::GAMEOVER;
}

eSceneType GameOverScene::Update()
{
	// 演出待ち（即操作させない）
	if (wait_timer > 0)
	{
		wait_timer--;
		return eSceneType::GAMEOVER;
	}

	InputManager* input = InputManager::GetInstance();

	// ↓
	if (input->GetButtonDown(XINPUT_BUTTON_DPAD_DOWN))
	{
		cursorIndex++;
		if (cursorIndex >= MENU_COUNT) cursorIndex = 0;
	}

	// ↑
	if (input->GetButtonDown(XINPUT_BUTTON_DPAD_UP))
	{
		cursorIndex--;
		if (cursorIndex < 0) cursorIndex = MENU_COUNT - 1;
	}

	// 決定
	if (input->GetButtonDown(XINPUT_BUTTON_A))
	{
		switch (cursorIndex)
		{
		case 0: return eSceneType::GAME_MAIN; // リトライ
		case 1: return eSceneType::TITLE;     // タイトル
		case 2: return eSceneType::EXIT;
		}
	}

	return eSceneType::GAMEOVER;
}

void GameOverScene::Draw()
{
	DrawString(600,80 , "GAME OVER", GetColor(255, 0, 0), SceneManager::font);

	// 待ち時間中は操作不可表示
	if (wait_timer > 0)
	{
		//SetFontSize(48);
		DrawString(100, 200, ".....", GetColor(255, 255, 255), SceneManager::font);
		return;
	}

	const char* menu[] = { "RETRY", "TITLE", "EXIT" };

	for (int i = 0; i < MENU_COUNT; i++)
	{
		if (i == cursorIndex)
		{
			DrawFormatStringToHandle(
				100,
				250 + i * 40,
				GetColor(255, 255, 0),
				SceneManager::font,
				"> %s",
				menu[i]
			);
		}
		else
		{
			DrawFormatStringToHandle(
				120,
				250 + i * 40,
				GetColor(255, 255, 255),
				SceneManager::font,
				"%s",
				menu[i]
			);
		}
	}
}
