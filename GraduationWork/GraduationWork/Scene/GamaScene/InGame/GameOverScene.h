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

public:
	GameOverScene();

	virtual ~GameOverScene();

	eSceneType GetNowSceneType() const override;
	eSceneType Update() override;
	void Draw() override;
};

