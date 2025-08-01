#pragma once
#include "../../SceneBase.h"
#include "../../../common.h"

#include "../../../Object/ObjectInfo.h"
#include "../../../Object/ObjectManager.h"

#include "../../../Utility/TileSet.h" 


class InGameScene :
    public SceneBase
{
private:
	int stage_width_num;    //ステージの横のブロック数
	int stage_height_num;   //ステージの縦のブロック数

	std::vector<std::vector<int>> stage_data; //ステージデータを格納する2次元ベクター

	ObjectManager object_manager; //オブジェクト管理クラス

	TileSet tile_set; //タイルセット
public:
	InGameScene();
	~InGameScene();

	void Initialize() override;		//初期化処理
	eSceneType Update() override;	//更新処理
	void Draw() override;		//描画処理
	void Finalize() override;		//終了時処理

	eSceneType GetNowSceneType()const override;

	//ステージデータを読み込む
	void LoadStage();

	//ステージデータを生成
	void SetStage();

	//カメラの位置を更新
	void UpdateCamera();
};

