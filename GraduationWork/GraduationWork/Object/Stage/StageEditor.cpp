#include "StageEditor.h"
#include <DxLib.h>
#include <algorithm>
#include "../../Utility/InputManager.h"
#include "../../Utility/UserTemplate.h"
#include "../../common.h"
#include <cassert>

#include "../ObjectInfo.h"

StageEditor::StageEditor(int _grid_size, StageData* _stage_data)
    : grid_size(_grid_size),
    stage_data(_stage_data),
    tile_set(nullptr),
    camera_offset{ 0,0 },
    selected_tile_id(0),
    ui_pos{ 0,0 },
    ui_size{ 0,0 },
    ui_dragging(false),
    ui_drag_offset{ 0,0 },
    ui_scroll_offset(0),
    max_scroll(0),
    tiles_per_row(2),
    tile_width(100),
    tile_height(80),
    ui_panel_width(0),
    ui_panel_height(0),
    hovered_grid_x(-1),
    hovered_grid_y(-1),
	current_mode(EditMode::Object)
{
    assert(stage_data != nullptr);

    width = stage_data->GetWidth();
    height = stage_data->GetHeight();
    grid_width = width;
    grid_height = height;

    // タイルセット初期化（既定のパスを使う。必要なら引数化してください）
    tile_set = new TileSet("Resource/Images/Tiles/tile.png", BLOCK_SIZE, BLOCK_SIZE);
    tile_set->LoadFromJson("Resource/Images/Tiles/tile.json");

    // tile_ids を作る
    int count = tile_set->GetTileCount();
    tile_ids.reserve(count);
    for (int i = 0; i < count; ++i) tile_ids.push_back(i);

    // UIパネル幅・高さ計算（余白込み）
    ui_panel_width = tiles_per_row * tile_width + 20;
    ui_panel_height = SCREEN_HEIGHT - 20;

    // UIは右側に固定
    ui_pos.x = static_cast<float>(SCREEN_WIDTH) - ui_panel_width;
    ui_pos.y = 0;
    ui_size.x = static_cast<float>(ui_panel_width);
    ui_size.y = static_cast<float>(ui_panel_height + 20); // ヘッダ分含む

    selection_box.x = static_cast<int>(ui_pos.x);
    selection_box.y = 20; // ヘッダ下
    selection_box.width = ui_panel_width;
    selection_box.height = ui_panel_height;

    selected_object_type = NONE;
    for (int i = 0; i < OBJECTTYPE_COUNT; ++i) {
        object_types.push_back(static_cast<eObjectType>(i));
    }
}

StageEditor::~StageEditor()
{
    delete tile_set;
}

void StageEditor::Initialize() 
{

}
void StageEditor::Finalize() {}

void StageEditor::Update(Vector2D& offset)
{
    // offset は呼び出し元のカメラ位置（world->screen 用）なので同期して最後に書き戻す
    camera_offset = offset;

    InputManager* input = InputManager::GetInstance();
    Cursor c = input->GetMouseCursor();
    Vector2D mouse_pos{ (float)c.x, (float)c.y };

    bool ui_handled = false;

    // ← モード切り替え
    if (input->GetKeyDown(KEY_INPUT_TAB))
    {
        current_mode = (current_mode == EditMode::Tile) ? EditMode::Object : EditMode::Tile;
    }

    // UI 入力処理（スクロール/選択）
    HandleUIInput(mouse_pos, ui_handled);

    // グリッド編集（UI 操作が優先される）
    HandleGridEditing(mouse_pos, ui_handled);

    // カメラ移動（UI の外でのみ）
    HandleCameraMovement(!ui_handled);

    // 書き戻し（外部の camera offset を更新）

    offset = camera_offset;
}

void StageEditor::Draw(Vector2D /*offset*/)
{
    DrawGrid();
    DrawFreeTiles();
    DrawUI();
    DrawTiles();
    DrawScrollBar();
}

void StageEditor::HandleUIInput(const Vector2D& mouse_pos, bool& out_ui_handled)
{
    InputManager* input = InputManager::GetInstance();

    if (current_mode == EditMode::Tile) {
        int rows = (tile_set->GetTileCount() + tiles_per_row - 1) / tiles_per_row;
        int content_height = rows * tile_height;
        max_scroll = Max(0, content_height - selection_box.height);

        int scroll_delta = 0;
        if (input->GetKey(KEY_INPUT_UP)) scroll_delta = -tile_height;
        if (input->GetKey(KEY_INPUT_DOWN)) scroll_delta = +tile_height;
        if (scroll_delta != 0)
            ui_scroll_offset = Clamp(ui_scroll_offset + scroll_delta, 0, max_scroll);

        int y_start = selection_box.y - ui_scroll_offset;
        for (int i = 0; i < tile_set->GetTileCount(); ++i) {
            int row = i / tiles_per_row;
            int col = i % tiles_per_row;
            int x = selection_box.x + col * tile_width + 5;
            int y = y_start + row * tile_height;

            bool over_button = (mouse_pos.x >= x && mouse_pos.x <= x + tile_width - 5 &&
                mouse_pos.y >= y && mouse_pos.y <= y + tile_height - 5);
            if (over_button && input->GetMouseDown(MOUSE_INPUT_LEFT)) {
                selected_tile_id = i;
                out_ui_handled = true;
            }
        }
    }
    else { // Objectモード
        int y = selection_box.y;
        for (int i = 0; i < OBJECTTYPE_COUNT; ++i) {
            bool over_button = (mouse_pos.x >= selection_box.x + 5 &&
                mouse_pos.x <= selection_box.x + selection_box.width - 10 &&
                mouse_pos.y >= y &&
                mouse_pos.y <= y + 40);
            if (over_button && input->GetMouseDown(MOUSE_INPUT_LEFT)) {
                selected_object_type = static_cast<eObjectType>(i);
                out_ui_handled = true;
            }
            y += 50;
        }
    }
}

void StageEditor::HandleGridEditing(const Vector2D& mouse_pos, bool ui_handled)
{
    hovered_grid_x = static_cast<int>((mouse_pos.x + camera_offset.x) / grid_size);
    hovered_grid_y = static_cast<int>((mouse_pos.y + camera_offset.y) / grid_size);

    bool is_over_ui = (mouse_pos.x >= ui_pos.x && mouse_pos.x <= ui_pos.x + ui_panel_width &&
        mouse_pos.y >= ui_pos.y && mouse_pos.y <= ui_pos.y + ui_panel_height + 20);
    if (ui_handled || is_over_ui) {
        hovered_grid_x = -1;
        hovered_grid_y = -1;
        return;
    }

    InputManager* input = InputManager::GetInstance();

    if (current_mode == EditMode::Tile) {
       /* if (input->GetMouse(MOUSE_INPUT_LEFT) && selected_tile_id >= 0)
        {
            Vector2D world_pos = mouse_pos + camera_offset;
            stage_data->AddPlacedTile(selected_tile_id, world_pos);
        }
        if (input->GetMouse(MOUSE_INPUT_RIGHT))
        {
            Vector2D world_pos = mouse_pos + camera_offset;
            stage_data->RemovePlacedTileNear(world_pos, tile_set->GetTileWidth() / 2.0f);
        }*/
        Vector2D world_pos = mouse_pos + camera_offset;

        if (preview_tile.active) {
            // ドラッグで位置更新
            preview_tile.pos = world_pos;

            // 左クリックで確定（押した瞬間だけ）
            if (input->GetMouseDown(MOUSE_INPUT_LEFT)) {
                stage_data->AddPlacedTile(preview_tile.tile_id, preview_tile.pos);
                preview_tile.active = false;
            }
            // 右クリックでキャンセル
            if (input->GetMouseDown(MOUSE_INPUT_RIGHT)) {
                preview_tile.active = false;
            }
        }
        else {
            // UIからタイル選択時にプレビュー開始
            if (selected_tile_id >= 0 && input->GetMouseDown(MOUSE_INPUT_LEFT)) {
                preview_tile.tile_id = selected_tile_id;
                preview_tile.pos = world_pos;
                preview_tile.active = true;
                preview_tile.scale = 1.0f;
            }

            // 右クリックで既存タイルを削除
            if (input->GetMouseDown(MOUSE_INPUT_RIGHT)) {
                stage_data->RemovePlacedTileNear(world_pos, tile_set->GetTileWidth() / 2.0f);
            }
        }
    }
    else if (current_mode == EditMode::Object) {
        if (hovered_grid_x >= 0 && hovered_grid_y >= 0) {
            if (input->GetMouse(MOUSE_INPUT_LEFT))
                stage_data->SetObj(hovered_grid_x, hovered_grid_y, static_cast<int>(selected_object_type));
            if (input->GetMouse(MOUSE_INPUT_RIGHT))
                stage_data->SetObj(hovered_grid_x, hovered_grid_y, 0);
        }
    }
}

void StageEditor::HandleCameraMovement(bool allow)
{
    if (!allow) return;
    InputManager* input = InputManager::GetInstance();
    float cam_speed = 8.0f;
    if (input->GetKey(KEY_INPUT_W)) camera_offset.y -= cam_speed;
    if (input->GetKey(KEY_INPUT_S)) camera_offset.y += cam_speed;
    if (input->GetKey(KEY_INPUT_A)) camera_offset.x -= cam_speed;
    if (input->GetKey(KEY_INPUT_D)) camera_offset.x += cam_speed;
}

void StageEditor::DrawGrid()
{
    // 自由配置中ならグリッド非表示
    if (current_mode == EditMode::Tile)
    {
        //for (int y = 0; y < grid_height; ++y)
        //{
        //    for (int x = 0; x < grid_width; ++x)
        //    {
        //        int draw_x = x * grid_size - static_cast<int>(camera_offset.x);
        //        int draw_y = y * grid_size - static_cast<int>(camera_offset.y);

        //        int id = stage_data->GetTile(x, y);
        //        if (id >= 0 && tile_set->HasTile(id))
        //        {
        //            tile_set->DrawTile(id, draw_x, draw_y);
        //        }

        //        // ID表示は任意
        //        DrawFormatString(draw_x, draw_y, GetColor(255, 255, 0), "%d", id);
        //    }
        //}
    }
    else if (current_mode == EditMode::Object)
    {
        for (int y = 0; y < grid_height; ++y)
        {
            for (int x = 0; x < grid_width; ++x)
            {
                int draw_x = x * grid_size - static_cast<int>(camera_offset.x);
                int draw_y = y * grid_size - static_cast<int>(camera_offset.y);

                eObjectType type = static_cast<eObjectType>(stage_data->GetObj(x, y));
                auto info = GetTypeInfo(type);
                DrawBox(draw_x, draw_y, draw_x + grid_size, draw_y + grid_size, info.color, TRUE);
                DrawString(draw_x + 2, draw_y + 2, info.name, GetColor(0, 0, 0));
            }
        }
        // ホバー表示は残す
        if (hovered_grid_x >= 0 && hovered_grid_y >= 0 &&
            hovered_grid_x < grid_width && hovered_grid_y < grid_height)
        {
            int hx0 = hovered_grid_x * grid_size - static_cast<int>(camera_offset.x);
            int hy0 = hovered_grid_y * grid_size - static_cast<int>(camera_offset.y);
            int hx1 = hx0 + grid_size;
            int hy1 = hy0 + grid_size;
            DrawBox(hx0, hy0, hx1, hy1, GetColor(200, 200, 255), FALSE);
        }
    }

   
}

void StageEditor::DrawUI()
{
    // ヘッダー
    const int header_h = 20;
    DrawBox(static_cast<int>(ui_pos.x), static_cast<int>(ui_pos.y),
        static_cast<int>(ui_pos.x) + selection_box.width,
        static_cast<int>(ui_pos.y) + header_h, GetColor(80, 80, 80), TRUE);
    DrawString(static_cast<int>(ui_pos.x) + 6, static_cast<int>(ui_pos.y) + 4, "Stage Editor", GetColor(255, 255, 255));

    const char* mode_text = (current_mode == EditMode::Tile) ? "Mode:TILE" : "Mode:OBJ";
    DrawFormatString((int)ui_pos.x + 6, (int)ui_pos.y + 4, GetColor(255, 255, 255),
        "Stage Editor [%s]", mode_text);

    // 背景（白）
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, 200);
    DrawBox(selection_box.x, selection_box.y,
        selection_box.x + selection_box.width,
        selection_box.y + selection_box.height,
        GetColor(255, 255, 255), TRUE);
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

    // 枠線
    DrawBox(selection_box.x, selection_box.y,
        selection_box.x + selection_box.width,
        selection_box.y + selection_box.height,
        GetColor(0, 0, 0), FALSE);
}

void StageEditor::DrawTiles()
{
    if (current_mode == EditMode::Tile) {
        // タイルリスト描画
        int y_start = selection_box.y - ui_scroll_offset;
        int button_w = tile_width - 5;
        int button_h = tile_height - 5;

        for (size_t i = 0; i < tile_ids.size(); ++i) {
            int row = static_cast<int>(i) / tiles_per_row;
            int col = static_cast<int>(i) % tiles_per_row;
            int x = selection_box.x + col * tile_width + 5;
            int y = y_start + row * tile_height;

            if (y + button_h < selection_box.y || y > selection_box.y + selection_box.height) continue;

            bool selected = (selected_tile_id == tile_ids[i]);
            int bg = selected ? GetColor(255, 255, 0) : GetColor(255, 255, 255);

            SetDrawBlendMode(DX_BLENDMODE_ALPHA, 200);
            DrawBox(x, y, x + button_w, y + button_h, bg, TRUE);
            SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
            DrawBox(x, y, x + button_w, y + button_h, GetColor(0, 0, 0), FALSE);

            TileInfo const* info = tile_set->GetTileInfo(tile_ids[i]);
            if (info) {
                tile_set->DrawTile(tile_ids[i], x + 5, y + 5);
            }
            DrawFormatString(x + 60, y + 10, GetColor(0, 0, 0), "%d", tile_ids[i]);
        }
    }
    else {
        // オブジェクトリスト描画
        int y = selection_box.y;
        int button_width = selection_box.width - 10;
        int button_height = 40;
        for (size_t i = 0; i < object_types.size(); ++i) {
            auto info = GetTypeInfo(object_types[i]);
            int color = (selected_object_type == object_types[i]) ? GetColor(255, 255, 0) : info.color;

            SetDrawBlendMode(DX_BLENDMODE_ALPHA, 200);
            DrawBox(selection_box.x + 5, y, selection_box.x + 5 + button_width, y + button_height, color, TRUE);
            SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
            DrawBox(selection_box.x + 5, y, selection_box.x + 5 + button_width, y + button_height, GetColor(0, 0, 0), FALSE);
            DrawFormatString(selection_box.x + 10, y + 10, GetColor(255, 255, 255), "%s", info.name);
            y += 50;
        }
    }
}

void StageEditor::DrawFreeTiles()
{
    // 確定済みタイル描画
    for (const auto& tile : stage_data->GetFreeTiles())
    {
        int draw_x = static_cast<int>(tile.pos.x - camera_offset.x);
        int draw_y = static_cast<int>(tile.pos.y - camera_offset.y);
        tile_set->DrawTile(tile.tile_id, draw_x, draw_y);
    }

    // プレビュー描画
    if (preview_tile.active) {
        int draw_x = static_cast<int>(preview_tile.pos.x - camera_offset.x);
        int draw_y = static_cast<int>(preview_tile.pos.y - camera_offset.y);
        tile_set->DrawTile(preview_tile.tile_id, draw_x, draw_y);
    }
}

void StageEditor::DrawScrollBar()
{
    if (max_scroll <= 0) return;

    int bar_w = 6;
    int bar_h = Max(8, (selection_box.height * selection_box.height) / (selection_box.height + max_scroll));
    int bar_x = selection_box.x + selection_box.width - 10;
    int bar_y = selection_box.y + (ui_scroll_offset * (selection_box.height - bar_h)) / Max(1, max_scroll);

    DrawBox(bar_x, bar_y, bar_x + bar_w, bar_y + bar_h, GetColor(0, 0, 0), TRUE);
}
