#include "InGameScene.h"
#include "DxLib.h"

#include <fstream>
#include <sstream>
#include <iostream>

InGameScene::InGameScene() :stage_width_num(0), stage_height_num(0), stage_data()
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

eSceneType InGameScene::Update( )
{
	object_manager.Update();
	UpdateCamera(); 

	return __super::Update();
}

void InGameScene::Draw()
{
	// 描画処理
	__super::Draw();
	object_manager.Draw(camera_location, 1.0);

	DrawString(200, 0, "GameMain", GetColor(255, 255, 255));


	DrawFormatString(10, 90, GetColor(255,255,255), "Camera Location: (%3f, %3f)", camera_location.x, camera_location.y);
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
	GameObject* player = object_manager.FindObjectType(eObjectType::PLAYER);
	if (!player) return;

	float screen_half_width = SCREEN_WIDTH / 2.0f;
	float stage_limit_left = 0.0f;
	float stage_limit_right = static_cast<float>(100) * BLOCK_SIZE - SCREEN_WIDTH;

	camera_location.x = player->GetLocation().x - screen_half_width;

	if (camera_location.x < stage_limit_left) camera_location.x = stage_limit_left;
	if (camera_location.x > stage_limit_right) camera_location.x = stage_limit_right;
}

