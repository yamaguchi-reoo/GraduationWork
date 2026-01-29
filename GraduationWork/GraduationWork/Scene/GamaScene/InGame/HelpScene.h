#pragma once
#include "../../SceneBase.h"
#include "../../../common.h"

enum class HelpCategory
{
	Control,
	Gimmick,
	Item,
	Max
};

class HelpScene:
	public SceneBase
{
private:
	HelpCategory current_category;
	int cursor_index;

	int help_font;

public:
	HelpScene();
	~HelpScene();

	void Initialize() override;		//初期化処理
	eSceneType Update() override;	//更新処理
	void Draw() override;			//描画処理
	void Finalize() override;		//終了時処理

	eSceneType GetNowSceneType()const override;

	// ヘルプ内容描画
	void DrawControlHelp();
	void DrawGimmickHelp();
	void DrawItemHelp();
};

