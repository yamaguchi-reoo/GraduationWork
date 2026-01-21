#pragma once
#include "../../SceneBase.h"

class TitleScene :public SceneBase
{
private:
	enum class TITLE_MENU {
		START,
		HELP,
		END
	};

	int MENU_COUNT;
	int cursorIndex;
	int Title_Img;

	float blinkAlpha;   
	bool blinkUp;       

	bool isFading;
	float fadeAlpha;
	eSceneType nextScene;

	int fadeDelayTimer;   // 待機用タイマー（フレーム）
	const int FADE_DELAY = 60; // 60FPS想定で1秒




public:

	TitleScene();
	virtual ~TitleScene();
	eSceneType Title_Select();
	eSceneType GetNowSceneType()const override;
	eSceneType Update() override;	//更新処理
	virtual void Draw()  override;
};

