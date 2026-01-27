#pragma once
#include "../../SceneBase.h"

class GameClear :public SceneBase
{
private:
	enum class GameClear_MENU {
		START,
		TITLE,
		END
	};

	int MENU_COUNT;
	int cursorIndex;
	int Title_Img;

	// ===== 演出用 =====
	int frame;
	int titleAlpha;
	int menuAlpha;
	bool isMenuVisible;
	bool isInputEnabled;
public:

	GameClear();
	virtual ~GameClear();
	eSceneType GameClear_Select();
	eSceneType GetNowSceneType()const override;
	eSceneType Update() override;	//更新処理
	virtual void Draw()  override;
};

