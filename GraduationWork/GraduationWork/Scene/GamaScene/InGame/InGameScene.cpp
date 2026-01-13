#include "InGameScene.h"
#include <DxLib.h>

#include <fstream>
#include <sstream>
#include <iostream>
#include <string>

#include "../../../Utility/InputManager.h"


InGameScene::InGameScene() :stage_width_num(0), stage_height_num(0), stage_data(0, 0),
tile_set("Resource/Images/Tiles/tiles_spritesheet.png", BLOCK_SIZE, BLOCK_SIZE),editor(nullptr),
edit_mode(false),stage_id(0)
{
	// JSONからタイルセットを読み込み
	//tile_set.LoadFromJson("Resource/Images/Tiles/tile.json"); 
	tile_set.LoadFromXML("Resource/Images/Tiles/tiles_spritesheet.xml"); 

	background_handle = LoadGraph("Resource/images/BackGround/background 1.png");

}

InGameScene::~InGameScene()
{
}

void InGameScene::Initialize()
{
	camera_location = Vector2D(0.0f, 0.0f); // カメラの初期位置を設定
	object_manager.Initialize(); // オブジェクト管理クラスの初期化

    stage_id = 1;

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
			stage_data.SaveCSV(stage_name);
			stage_data.SaveTileCSV(tile_name);

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

	GoalPoint* goal = (GoalPoint*)object_manager.FindObjectType(eObjectType::GOALPOINT);
    if(goal && goal->reached)
    {
        // ゴールに到達したら次のステージへ
        stage_id++;
        // オブジェクトを一旦クリア
        object_manager.Finalize();    // オブジェクト解放処理
        object_manager.Initialize();  // 初期化
        // ステージデータ読み込み
        LoadStage();
	}

	return __super::Update();
}

void InGameScene::Draw()
{
    DrawBox(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, GetColor(185, 185, 185), TRUE);
    DrawBackground();

    // 通常描画
    __super::Draw();
    object_manager.Draw(camera_location, 1.0);

    DrawTiles();

    // --- Blend演出（闇・光の広がり）---
    DirectionScreen();

    // --- Blendの影響を受けないオブジェクトを再描画 ---
    object_manager.DrawNoBlendObjects(camera_location, 1.0);

    // --- 編集モードの表示 ---
    if (edit_mode)
    {
        SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
        DrawBox(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, GetColor(0, 0, 0), TRUE);
        SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
        editor->Draw(camera_location);
        DrawString(600, 10, "EDIT MODE", GetColor(255, 255, 0));
    }
    else
    {
        DrawString(600, 10, "GAME MODE", GetColor(255, 255, 255));

        // --- 追加表示 ---
        std::string stage_label = "Stage " + std::to_string(stage_id);
        DrawString(600, 40, stage_label.c_str(), GetColor(255, 255, 255));
    }
}


void InGameScene::Finalize()
{
	// 終了時処理
	__super::Finalize();
	plates.clear();
	lights.clear();
	object_manager.Finalize();
	if (editor)
	{
		editor->Finalize();
		delete editor;
		editor = nullptr;
	}
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

			int obj = stage_data.GetObj(x, y);
			if (obj == static_cast<int>(eObjectType::TEXT))
			{
				int draw_x = x * BLOCK_SIZE - static_cast<int>(camera_location.x);
				int draw_y = y * BLOCK_SIZE - static_cast<int>(camera_location.y);
				DrawFormatString(draw_x, draw_y - 20, GetColor(255, 255, 255), "PUSH RB");
			}
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

void InGameScene::DirectionScreen()
{
    static int dark_alpha = 0;     // 通常のうっすら暗くする用
    static int shock_alpha = 0;    // 切替直後のショック(強調)
    static int ripple_frame = 0;   // 波紋寿命 0 なら非表示
    static bool ripple_is_dark = true;

    static int pop_frame = 0;      // 小円ポップ演出
    static bool pop_is_dark = true;

    Player* player = (Player*)object_manager.FindObjectType(eObjectType::PLAYER);
    if (!player) return;

    static bool prev_shadow = false;
    bool now_shadow = player->IsPlayerShadow();

    // 状態が変わった瞬間
    if (now_shadow != prev_shadow)
    {
        // 波紋スタート（8〜10フレーム）
        ripple_frame = 1;
        ripple_is_dark = now_shadow; // 影なら黒、実なら白

        // 小円スタート（3フレーム）
        pop_frame = 1;
        pop_is_dark = now_shadow;

        // グラデーションを強調（瞬間的なショック）
        shock_alpha = 40;

        prev_shadow = now_shadow;
    }

    // 既存の常時うっすら暗くする処理
    const int fade_speed = 8;
    const int max_dark = 110;

    if (now_shadow) dark_alpha = Min(dark_alpha + fade_speed, max_dark);
    else            dark_alpha = Max(dark_alpha - fade_speed, 0);

    // ショック α を減衰
    shock_alpha = Max(shock_alpha - 8, 0);

    int total_alpha = Clamp(dark_alpha + shock_alpha, 0, 255);

    // 画面グラデ
    if (total_alpha > 0)
    {
        SetDrawBlendMode(DX_BLENDMODE_ALPHA, total_alpha);
        DrawBox(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, GetColor(0, 0, 0), TRUE);
        SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
    }

    // 波紋
    if (ripple_frame > 0)
    {
        int life = 10; // 10フレームで終了
        float t = (float)ripple_frame / life;

        // EaseOut
        float eased = 1.0f - powf(1 - t, 3);

        float radius = eased * 1400.0f; // 画面対角より少し大きく

        int alpha = (int)(150 * (1.0f - t)); // だんだん薄く

        Vector2D c = player->GetLocation() + (player->GetBoxSize() / 2.0f) - camera_location;

        SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
        int col = ripple_is_dark ? GetColor(0, 0, 0) : GetColor(255, 255, 255);
        DrawCircle((int)c.x, (int)c.y, (int)radius, col, FALSE);
        SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

        ripple_frame++;
        if (ripple_frame > life) ripple_frame = 0;
    }

	// 小円ポップ
    if (pop_frame > 0)
    {
        int life = 3; // 3フレームだけ

        float r = 50.0f * (1.0f + pop_frame * 0.2f); // 少し大きくなる
        int alpha = 180 - pop_frame * 50;           // だんだん薄く

        Vector2D c = player->GetLocation() + (player->GetBoxSize() / 2.0f) - camera_location;
        int col = pop_is_dark ? GetColor(0, 0, 0) : GetColor(255, 255, 255);

        SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
        DrawCircle((int)c.x, (int)c.y, (int)r, col, TRUE);
        SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

        pop_frame++;
        if (pop_frame > life) pop_frame = 0;
    }
}

void InGameScene::LoadStage()
{
    stage_name = "Resource/File/Stage" + std::to_string(stage_id) + ".csv";
    tile_name = "Resource/File/tile" + std::to_string(stage_id) + ".csv";

	if (!stage_data.LoadCSV(stage_name))
	{
		std::cerr << "ステージファイルを開けませんでした\n";
		return;
	}

	if (!stage_data.LoadTileCSV(tile_name))
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

    // まずPlayerを生成
    for (int y = 0; y < stage_data.GetHeight(); ++y)
    {
        for (int x = 0; x < stage_data.GetWidth(); ++x)
        {
            int obj = stage_data.GetObj(x, y);
            if (obj == PLAYER)
            {
                Vector2D world_pos(x * (float)BLOCK_SIZE, y * (float)BLOCK_SIZE);
                object_manager.CreateObject<Player>(world_pos, Vector2D(block_size.x - 10, 50.f));
            }
        }
    }

    // 次にその他のオブジェクトを生成
    for (int y = 0; y < stage_data.GetHeight(); ++y)
    {
        for (int x = 0; x < stage_data.GetWidth(); ++x)
        {
            Vector2D world_pos(x * (float)BLOCK_SIZE, y * (float)BLOCK_SIZE);
            int obj = stage_data.GetObj(x, y);

            switch (obj)
            {
            case NONE:
            case PLAYER: // もう生成済みなのでスキップ
                break;
            case BLOCK:
                object_manager.CreateObject<Block>(world_pos, block_size);
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
			case HEAL:
				object_manager.CreateObject<Heal>(world_pos, Vector2D(48.0f, 48.0f));
				break;
			case SHADOWHEAL:
				object_manager.CreateObject<ShadowHeal>(world_pos, Vector2D(48.0f, 48.0f));
                break;
                case TEXT:
                object_manager.CreateObject<TextObject>(world_pos, Vector2D(48.0f, 96.0f));
                break;
			case GOALPOINT:
				object_manager.CreateObject<GoalPoint>(world_pos, Vector2D(48.0f, 96.0f));
				break;
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

void InGameScene::DrawBackground()
{ 
	// 背景画像のサイズを取得
	int bg_w, bg_h;
	GetGraphSize(background_handle, &bg_w, &bg_h);

	int offset_x = static_cast<int>(camera_location.x) % bg_w;
	if (offset_x < 0) offset_x += bg_w;

	for (int x = -offset_x; x < SCREEN_WIDTH; x += bg_w)
	{
		DrawGraph(x, 0, background_handle, TRUE);
	}

	// ---- �w�i������Â����� ----
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100);  // ��=100�Ŕ������̍�
	DrawBox(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, GetColor(0, 0, 0), TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0); // �u�����h���
}
