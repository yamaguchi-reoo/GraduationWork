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

	isFading = false;
	fadeAlpha = 0.0f;

	fadeDelayTimer = 0;

	GameOver_img = LoadGraph("Resource/Images/GameOver/GameOver.png");
	nextScene = eSceneType::GAMEOVER;


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

	// 待機タイマー処理
	if (fadeDelayTimer > 0)
	{
		fadeDelayTimer--;
		return eSceneType::GAMEOVER;
	}

	// フェード処理（遷移予約があるときのみ）
	if (nextScene != eSceneType::GAMEOVER)
	{
		isFading = true;
		fadeAlpha += 1.5f;
		if (fadeAlpha >= 255.0f)
		{
			fadeAlpha = 255.0f;
			SoundManager::GetInstance()->Stop(SoundID::GAME_OVER_BGM);
			return nextScene;
		}
	}

	InputManager* input = InputManager::GetInstance();

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

	if (input->GetButtonDown(XINPUT_BUTTON_A))
	{
		SoundManager::GetInstance()->Stop(SoundID::GAME_OVER_BGM);
		switch (cursorIndex)
		{
		case 0: // RETRY
			nextScene = eSceneType::GAME_MAIN;
			fadeDelayTimer = FADE_DELAY;
			SoundManager::GetInstance()->Play(SoundID::PUSH);
			break;

		case 1: // TITLE
			nextScene = eSceneType::TITLE;
			fadeDelayTimer = FADE_DELAY;
			SoundManager::GetInstance()->Play(SoundID::PUSH);
			break;

		case 2: // EXIT
			nextScene = eSceneType::EXIT;
			fadeDelayTimer = FADE_DELAY;
			SoundManager::GetInstance()->Play(SoundID::PUSH);
			break;
		}
	}

	return eSceneType::GAMEOVER;
}

void GameOverScene::Draw()
{

	//ゲームオーバー画像
	DrawGraph(-50, 0, GameOver_img, TRUE);


	// 画面全体を少し暗くする
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 120); // 0?255（数値が大きいほど暗くなる）
	DrawBox(0, 0, 1280, 720, GetColor(0, 0, 0), TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	// 文字幅を取得
	int textWidth = GetDrawStringWidthToHandle(text, strlen(text), SceneManager::titleFont);

	// 中央より少し上に配置
	int x = (SCREEN_W - textWidth) / 2;
	int y = SCREEN_H / 2 - 180;

	DrawStringToHandle(x-40, y, text, GetColor(255, 0, 0), SceneManager::titleFont);


	// 待ち時間中は操作不可表示
	if (wait_timer > 0)
	{
		//SetFontSize(48);
		DrawString(100, 200, ".....", GetColor(255, 255, 255), SceneManager::font);
		return;
	}

	int centerX = SCREEN_W / 2;
	int centerY = SCREEN_H / 2;

	const char* menu[] = { "RETRY", "TITLE", "EXIT" };

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

	if (isFading)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, (int)fadeAlpha);
		DrawBox(0, 0, 1280, 720, GetColor(0, 0, 0), TRUE);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}
}
