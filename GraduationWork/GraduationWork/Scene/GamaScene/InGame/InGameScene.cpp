#include "InGameScene.h"
#include "DxLib.h"

#include <fstream>
#include <sstream>
#include <iostream>

GameMainScene::GameMainScene()
{
}

GameMainScene::~GameMainScene() 
{
}

void GameMainScene::Initialize()
{
	// 初期化処理
	LoadStage();
}

eSceneType GameMainScene::Update()
{
	// 更新処理
	UpdateCamera();
	FindPlayer();
	return __super::Update();
}

void GameMainScene::Draw()
{
	// 描画処理
	__super::Draw();
}

void GameMainScene::Finalize()
{
	// 終了時処理
	__super::Finalize();
}

eSceneType GameMainScene::GetNowSceneType() const
{
	return eSceneType::GAME_MAIN;
}

void GameMainScene::LoadStage()
{
}

void GameMainScene::SetStage()
{
}

void GameMainScene::UpdateCamera()
{
	//プレイヤーが存在するならカメラを追従させる
	if (player)
	{
		float screen_half_width = SCREEN_WIDTH / 2;				//画面の半分の幅
		float stage_limit_left = 0.0f;							//ステージの左端
		float stage_limit_right = stage_width_num * BLOCK_SIZE - SCREEN_WIDTH; //ステージの右端 

		//カメラ位置 ＝ プレイヤーの位置 - 画面の半分の幅 
		camera_location.x = player->GetLocation().x - screen_half_width;

		//画面端ではスクロールしないよう制限
		if (camera_location.x < stage_limit_left) camera_location.x = stage_limit_left;
		if (camera_location.x > stage_limit_right) camera_location.x = stage_limit_right;
	}
}

void GameMainScene::FindPlayer()
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
