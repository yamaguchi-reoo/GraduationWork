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
	delete player; // プレイヤーオブジェクトのメモリ解放
}

void InGameScene::Initialize()
{
	camera_location = Vector2D(0.0f, 0.0f); // カメラの初期位置を設定

	// 初期化処理
	LoadStage();
}

eSceneType InGameScene::Update()
{
	// 更新処理
	FindPlayer();   // 先にプレイヤー取得
	UpdateCamera(); // それから追従
	return __super::Update();
}

void InGameScene::Draw()
{
	// 描画処理
	__super::Draw();

	DrawString(0, 0, "GameMain", GetColor(255, 255, 255));
}

void InGameScene::Finalize()
{
	// 終了時処理
	__super::Finalize();
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
	//プレイヤーオブジェクトを探して取得
	for (auto obj : objects)
	{
		if (obj->GetObjectType() == PLAYER)
		{
			player = obj;
			break;
		}
	}
}
