#include "InGameScene.h"
#include "DxLib.h"

#include "../../../Object/ObjectList.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include <string>


InGameScene::InGameScene() :stage_width_num(0), stage_height_num(0), stage_data(),
tile_set("Resource/Images/Tiles/tile.png", BLOCK_SIZE, BLOCK_SIZE)
{
	// JSONからタイルセットを読み込み
	tile_set.LoadFromJson("Resource/Images/Tiles/tile.json"); 
	
}

InGameScene::~InGameScene()
{
}

void InGameScene::Initialize()
{
	camera_location = Vector2D(0.0f, 0.0f); // カメラの初期位置を設定
	object_manager.Initialize(); // オブジェクト管理クラスの初期化

	// 初期化処理
	LoadStage();
}

eSceneType InGameScene::Update( )
{
	object_manager.Update();
	UpdateCamera(); 

	return __super::Update();
}

void InGameScene::Draw()
{
	//DrawBox(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, GetColor(255, 255, 255), TRUE); // 背景を黒く塗りつぶす
	// 描画処理
	__super::Draw();
	object_manager.Draw(camera_location, 1.0);

	/*std::vector<int> favorite_tiles = { 0, 5, 12, 25, 31, 45, 62, 78, 89, 105 };
	tile_set.DrawSelectedTiles(favorite_tiles, 10, 20, 5);*/


	DrawString(200, 0, "GameMain", GetColor(255, 255, 255));


	//DrawFormatString(10, 90, GetColor(255,255,255), "Camera Location: (%3f, %3f)", camera_location.x, camera_location.y);

	
}

void InGameScene::Finalize()
{
	// 終了時処理
	__super::Finalize();
	object_manager.Finalize();
}

eSceneType InGameScene::GetNowSceneType() const
{
	return eSceneType::GAME_MAIN;
}

void InGameScene::LoadStage()
{
	std::ifstream file("Resource/File/Stage.csv");
	if (!file)
	{
		std::cerr << "ステージファイルを開けませんでした: " << std::endl;
		return;
	}

	// 1行目を読み込んでステージ幅と高さを取得
	std::string line;

	//fileから1行読み込んで、line に格納。
	if (std::getline(file, line)) {
		//文字列を解析するためのストリームを作成。
		std::stringstream ss(line);
		std::string width, height;

		// カンマで分割して幅と高さを取得
		std::getline(ss, width, ',');
		std::getline(ss, height, ',');

		//文字列を整数に変換
		stage_width_num = std::stoi(width);   // ステージ幅
		stage_height_num = std::stoi(height); // ステージ高さ
	}

	stage_data.resize(stage_height_num);
	for (int i = 0; i < stage_height_num; ++i) {
		stage_data[i].resize(stage_width_num);
	}

	//ステージデータの読み込み（CSVの2行目以降）
	for (int i = 0; i < stage_height_num; i++) {
		//1行ずつ読み込む
		if (std::getline(file, line)) {
			std::stringstream ss(line);
			for (int j = 0; j < stage_width_num; j++) {
				//カンマ区切りでデータを取得
				std::string value;
				if (std::getline(ss, value, ',')) {
					//文字列を整数に変換してステージデータに格納
					stage_data[i][j] = std::stoi(value);
				}
			}
		}
	}

	file.close();

	SetStage();

}

void InGameScene::SetStage()
{
	//1ブロックの大きさ
	const Vector2D block_size((float)BLOCK_SIZE);

	for (int y = 0; y < stage_height_num; ++y)
	{
		for (int x = 0; x < stage_width_num; ++x)
		{
			int tile = stage_data[y][x];

			// ワールド座標の計算（左下原点でY座標を反転している例）
			Vector2D world_pos(x * BLOCK_SIZE, SCREEN_HEIGHT - ((stage_height_num - y) * BLOCK_SIZE));

			switch (tile)
			{
			case NONE: 
				break;
			case BLOCK: 
				object_manager.CreateObject<Block>(world_pos, block_size);
				break;
			case PLAYER: 
				object_manager.CreateObject<Player>(world_pos, Vector2D(48.0f, 64.0f));
				break;
			}
		}
	}

}

void InGameScene::UpdateCamera()
{
	GameObject* player = object_manager.FindObjectType(eObjectType::PLAYER);
	if (!player) return;

	float screen_half_width = SCREEN_WIDTH / 2.0f;
	float stage_limit_left = 0.0f;
	float stage_limit_right = static_cast<float>(100) * BLOCK_SIZE - SCREEN_WIDTH;

	camera_location.x = player->GetLocation().x - screen_half_width;

	if (camera_location.x < stage_limit_left) camera_location.x = stage_limit_left;
	if (camera_location.x > stage_limit_right) camera_location.x = stage_limit_right;
}

