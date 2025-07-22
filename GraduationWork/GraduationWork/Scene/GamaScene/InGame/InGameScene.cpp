#include "InGameScene.h"
#include "DxLib.h"

#include <fstream>
#include <sstream>
#include <iostream>

InGameScene::InGameScene() :stage_width_num(0), stage_height_num(0), stage_data(), player(nullptr)
{
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

eSceneType InGameScene::Update()
{
	// 更新処理
	if (!player)
	{
		FindPlayer();
	}

	object_manager.Update();
	UpdateCamera(); 

	return __super::Update();
}

void InGameScene::Draw()
{
	// 描画処理
	__super::Draw();
	object_manager.Draw(camera_location, 1.0);

	DrawString(0, 0, "GameMain", GetColor(255, 255, 255));
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
}

void InGameScene::SetStage()
{
}

void InGameScene::UpdateCamera()
{
	//プレイヤーが存在するならカメラを追従させる
	if (player)
	{
		float screen_half_width = SCREEN_WIDTH / 2;				//画面の半分の幅
		float stage_limit_left = 0.0f;							//ステージの左端
		float stage_limit_right = static_cast<float>(stage_width_num) * BLOCK_SIZE - SCREEN_WIDTH; //ステージの右端 

		//カメラ位置 ＝ プレイヤーの位置 - 画面の半分の幅 
		camera_location.x = player->GetLocation().x - screen_half_width;

		//画面端ではスクロールしないよう制限
		if (camera_location.x < stage_limit_left) camera_location.x = stage_limit_left;
		if (camera_location.x > stage_limit_right) camera_location.x = stage_limit_right;
	}
}

void InGameScene::FindPlayer()
{
	player = object_manager.FindObjectType(PLAYER);
}
