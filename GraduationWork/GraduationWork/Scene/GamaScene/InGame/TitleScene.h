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

public:

	TitleScene();
	virtual ~TitleScene();
	eSceneType Title_Select();
	eSceneType GetNowSceneType()const override;
	eSceneType Update() override;	//çXêVèàóù
	virtual void Draw()  override;
};

