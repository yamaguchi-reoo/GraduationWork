#include "InGameScene.h"
#include <DxLib.h>

#include <fstream>
#include <sstream>
#include <iostream>
#include <string>

#include "../../../Utility/InputManager.h"


InGameScene::InGameScene() :stage_width_num(0), stage_height_num(0), stage_data(0, 0),
tile_set("Resource/Images/Tiles/tile.png", BLOCK_SIZE, BLOCK_SIZE),editor(nullptr),
edit_mode(false)
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

	// ステージエディターの初期化
	editor = new StageEditor(BLOCK_SIZE, &stage_data);
}

eSceneType InGameScene::Update()
{
	InputManager* input = InputManager::GetInstance();

#ifdef _DEBUG
	// F1で編集モード切り替え
	if (input->GetKeyDown(KEY_INPUT_F1))
	{
		if (edit_mode)
		{
			// 編集モード終了 → 保存して再読み込み
			//editor->SaveStageData("stage.csv");
			stage_data.SaveCSV("Resource/File/Stage.csv");
			stage_data.SaveTileCSV("Resource/File/Tile.csv");

			// オブジェクトを一旦クリア
			object_manager.Finalize();    // オブジェクト解放処理
			object_manager.Initialize();  // 初期化

			// ステージデータ読み込み
			LoadStage();
		}

		edit_mode = !edit_mode;
	}
#endif

	if (edit_mode)
	{
		// 編集モード中：エディターだけ更新
		editor->Update(camera_location);
	}
	else
	{
		// 通常モード：オブジェクトを更新
		object_manager.Update(camera_location);
	}

	// カメラは両モードで更新
	UpdateCamera();

	return __super::Update();
}

void InGameScene::Draw()
{
	DrawBox(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, GetColor(185, 185, 185), TRUE); 
	// 通常描画
	__super::Draw();
	object_manager.Draw(camera_location, 1.0);

	DrawTiles();

	if (edit_mode)
	{
		// 背景を半透明で黒く塗る
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128); // 128 = 50%透明
		DrawBox(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, GetColor(0, 0, 0), TRUE);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0); // ブレンド解除

		// エディターのグリッド描画
		editor->Draw(camera_location);

		// 編集モード表示
		DrawString(600, 10, "EDIT MODE", GetColor(255, 255, 0));
	}
	else
	{
		DrawString(600, 10, "GAME MODE", GetColor(255, 255, 255));
	}

	/*std::vector<int> favorite_tiles = { 0, 5, 12, 25, 31, 45, 62, 78, 89, 105 };
	tile_set.DrawSelectedTiles(favorite_tiles, 10, 20, 5);*/	
}

void InGameScene::Finalize()
{
	// 終了時処理
	__super::Finalize();
	plates.clear();
	lights.clear();
	object_manager.Finalize();
	editor->Finalize();
}

eSceneType InGameScene::GetNowSceneType() const
{
	return eSceneType::GAME_MAIN;
}

void InGameScene::DrawTiles()
{
	for (int y = 0; y < stage_data.GetHeight(); ++y)
	{
		for (int x = 0; x < stage_data.GetWidth(); ++x)
		{
			int tile_id = stage_data.GetTile(x, y);
			if (tile_id <= 0) continue;

			int draw_x = x * BLOCK_SIZE - static_cast<int>(camera_location.x);
			int draw_y = y * BLOCK_SIZE - static_cast<int>(camera_location.y);

			if (tile_set.HasTile(tile_id))
				tile_set.DrawTile(tile_id, draw_x, draw_y);
		}
	}

	// 自由配置タイル描画
	for (const auto& tile : stage_data.GetFreeTiles())
	{
		int draw_x = static_cast<int>(tile.pos.x - camera_location.x);
		int draw_y = static_cast<int>(tile.pos.y - camera_location.y);
		tile_set.DrawTile(tile.tile_id, draw_x, draw_y);
	}

	/*for (auto& t : placed_tiles)
	{
		tile_set.DrawTile(t.tile_id, (int)(t.pos.x - camera_location.x), (int)(t.pos.y - camera_location.y));
	}*/
}

void InGameScene::LoadStage()
{
	if (!stage_data.LoadCSV("Resource/File/Stage.csv"))
	{
		std::cerr << "ステージファイルを開けませんでした\n";
		return;
	}

	if (!stage_data.LoadTileCSV("Resource/File/Tile.csv"))
	{
		std::cerr << "Tileファイルを開けませんでした\n";
		return;
	}

	SetStage(); // データをもとにオブジェクト生成
}

void InGameScene::SetStage()
{
	plates.clear();
	lights.clear();
	const Vector2D block_size((float)BLOCK_SIZE);

	for (int y = 0; y < stage_data.GetHeight(); ++y)
	{
		for (int x = 0; x < stage_data.GetWidth(); ++x)
		{
			// 左上原点で描画位置
			Vector2D world_pos(x * static_cast<float>(BLOCK_SIZE), y * static_cast<float>(BLOCK_SIZE));
			// オブジェクトレイヤー
			int obj = stage_data.GetObj(x, y);

			switch (obj)
			{
			case NONE:
				break;
			case BLOCK:
				object_manager.CreateObject<Block>(world_pos, block_size);
				break;
			case PLAYER:
				object_manager.CreateObject<Player>(world_pos, Vector2D(45.0f, 64.0f));
				break;
			case WALL:
				object_manager.CreateObject<Wall>(world_pos, Vector2D(48.0f, 48.0f));
				break;
			case LIGHT:
			{
				Light* light = object_manager.CreateObject<Light>(world_pos, Vector2D(30.0f, 100.0f));
				lights.push_back(light);
				break;
			}
			case INVISIBLEFLOOR:
				object_manager.CreateObject<Invisiblefloor>(world_pos, Vector2D(96.0f, 14.0f));
				break;
			case PUSHBLOCK:
				object_manager.CreateObject<PushBlock>(world_pos, Vector2D(48.0f, 48.0f));
				break;
			case ENEMY:
				object_manager.CreateObject<Enemy>(world_pos, Vector2D(48.0f, 64.0f));
				break;
			case REALENEMY:
				object_manager.CreateObject<RealEnemy>(world_pos, Vector2D(48.0f, 64.0f));
				break;
			case PLATE:
			{
				Plate* plate = object_manager.CreateObject<Plate>(world_pos, Vector2D(100.0f, 10.0f));
				plates.push_back(plate);
				break;
			}
			}
		}
	}
	// 生成後に紐付け
	for (size_t i = 0; i < plates.size() && i < lights.size(); ++i)
	{
		plates[i]->linked_light = lights[i];
	}
}

void InGameScene::UpdateCamera()
{
	if (edit_mode)
	{
		// 編集モードでは editor のカメラを使用
		camera_location = editor->GetCameraOffset();
	}
	else
	{
		GameObject* player = object_manager.FindObjectType(eObjectType::PLAYER);
		if (!player) return;

		float screen_half_width = SCREEN_WIDTH / 2.0f;
		float stage_limit_left = 0.0f;
		float stage_limit_right = static_cast<float>(stage_data.GetWidth()) * BLOCK_SIZE - SCREEN_WIDTH;

		camera_location.x = player->GetLocation().x - screen_half_width;

		if (camera_location.x < stage_limit_left) camera_location.x = stage_limit_left;
		if (camera_location.x > stage_limit_right) camera_location.x = stage_limit_right;
	}
}
