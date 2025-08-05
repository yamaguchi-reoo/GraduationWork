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
	int select;

	int MENU_COUNT;
	int cursorIndex;

public:

	TitleScene();
	virtual ~TitleScene();
	void Title_Input();
	eSceneType GetNowSceneType()const override;
	eSceneType Update() override;	//çXêVèàóù
	virtual void Draw()  override;
};

