#pragma once
#include "../../SceneBase.h"
class GameOverScene :public SceneBase
{
private:
	enum class GAMEOVER_MENU
	{
		RETRY,
		TITLE,
		EXIT
	};

	int MENU_COUNT;
	int cursorIndex;
	int wait_timer;
	int GameOver_img;

	bool isFading;
	float fadeAlpha;
	eSceneType nextScene;

	int fadeDelayTimer;   // 待機用タイマー（フレーム）
	const int FADE_DELAY = 60; // 60FPS想定で1秒

	const int SCREEN_W = 1280;
	const int SCREEN_H = 720;

	const char* text = "GAME OVER";



public:
	GameOverScene();

	virtual ~GameOverScene();

	void Initialize() override;
	eSceneType GetNowSceneType() const override;
	eSceneType Update() override;
	void Draw() override;
};

