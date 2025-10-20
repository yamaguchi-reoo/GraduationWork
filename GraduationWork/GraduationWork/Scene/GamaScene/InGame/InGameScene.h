#pragma once
#include "../../SceneBase.h"
#include "../../../common.h"

#include "../../../Object/ObjectInfo.h"
#include "../../../Object/ObjectManager.h"
#include "../../../Object/ObjectList.h"

#include "../../../Utility/TileSet.h" 
#include "../../../Object/Stage/StageEditor.h"
#include "../../../Object/Stage/StageData.h"

class InGameScene :
    public SceneBase
{
private:
	int stage_width_num;    //ステージの横のブロック数
	int stage_height_num;   //ステージの縦のブロック数

	StageData stage_data;		  //ステージデータを格納する2次元ベクター
	ObjectManager object_manager; //オブジェクト管理クラス
	TileSet tile_set;			  //タイルセット
	StageEditor* editor;		  //ステージエディタ
	
	std::vector<PlacedTile> placed_tiles; //配置されたタイル情報

	bool edit_mode;			      //ステージエディットモードかどうか

	//Object同士の紐づけ
	std::vector<Light*> lights;
	std::vector<Plate*> plates;

	int background_handle;
public:
	InGameScene();
	~InGameScene();

	void Initialize() override;		//初期化処理
	eSceneType Update() override;	//更新処理
	void Draw() override;			//描画処理
	void Finalize() override;		//終了時処理

	eSceneType GetNowSceneType()const override;
	
	void DrawTiles();

	//ステージデータを読み込む
	void LoadStage();

	//ステージデータを生成
	void SetStage();

	//カメラの位置を更新
	void UpdateCamera();

	//背景
	void DrawBackground();
};

