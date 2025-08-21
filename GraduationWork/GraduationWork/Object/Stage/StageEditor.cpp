#include "StageEditor.h"
#include <DxLib.h>

#include "../../Utility/InputManager.h"
#include "../../common.h"

#include <fstream>

StageEditor::StageEditor(int _grid_size, StageData* _stage_data)
    : grid_size(_grid_size), stage_data(_stage_data),
    selected_tile_type(NONE), tile_types(),
    ui_pos({ 50.0f, 50.0f }), ui_size{ 0 }, ui_dragging(false), ui_drag_offset({ 0,0 }),
    hovered_grid_x(-1), hovered_grid_y(-1)
{
    width = stage_data->GetWidth();
    height = stage_data->GetHeight();

    grid_width = width;
    grid_height = height;

    // タイルリストの初期化（表示順をTILE_INFOに従う）
    tile_types.clear();
    for (int i = 0; i < OBJECTTYPE_COUNT; ++i) // 0はNONEなのでスキップ
    {
        tile_types.push_back(static_cast<eObjectType>(i));
    }
}

StageEditor::~StageEditor()
{
}

void StageEditor::Initialize()
{
    // 必要ならロードなど
}

void StageEditor::CalcUIRect(int& out_w, int& out_h) const
{
    int tile_box_w = 120;
    int tile_box_h = static_cast<int>(tile_types.size()) * 50;
    out_w = tile_box_w;
    out_h = tile_box_h + 60;
}

void StageEditor::ClampUIToScreen()
{
    int ui_w, ui_h;
    CalcUIRect(ui_w, ui_h);

    if (ui_pos.x < 0) ui_pos.x = 0;
    if (ui_pos.y < 0) ui_pos.y = 0;
    if (ui_pos.x > SCREEN_WIDTH - ui_w) ui_pos.x = SCREEN_WIDTH - ui_w;
    if (ui_pos.y > SCREEN_HEIGHT - ui_h) ui_pos.y = SCREEN_HEIGHT - ui_h;
}

void StageEditor::Update(Vector2D& offset)
{
    InputManager* input = InputManager::GetInstance();
    Cursor c = input->GetMouseCursor();

    int header_height = 20;
    int tile_height = 50;
    ui_size.x = 120;
    ui_size.y = header_height + static_cast<int>(tile_types.size()) * tile_height + 20;

    selection_box.x = ui_pos.x;
    selection_box.y = ui_pos.y + header_height;
    selection_box.width = ui_size.x;
    selection_box.height = static_cast<int>(tile_types.size()) * tile_height;

    bool ui_handled = false;

    // --- UIタイル選択 ---
    int y = selection_box.y;
    for (size_t i = 0; i < tile_types.size(); ++i)
    {
        bool over_button = (c.x >= selection_box.x + 5 &&
            c.x <= selection_box.x + selection_box.width - 10 &&
            c.y >= y &&
            c.y <= y + 40);
        if (over_button && input->GetMouseDown(MOUSE_INPUT_LEFT))
        {
            selected_tile_type = tile_types[i];
            ui_handled = true;
        }
        y += 50;
    }

    // --- グリッド上のマウス座標を計算 ---
    hovered_grid_x = (c.x + camera_offset.x) / grid_size;
    hovered_grid_y = (c.y + camera_offset.y) / grid_size;

    if (ui_handled)
    {
        hovered_grid_x = -1;
        hovered_grid_y = -1;
    }

    // --- グリッド編集 ---
    if (!ui_handled && hovered_grid_x >= 0 && hovered_grid_x < grid_width &&
        hovered_grid_y >= 0 && hovered_grid_y < grid_height)
    {
        if (input->GetMouse(MOUSE_INPUT_LEFT) && selected_tile_type != NONE)
        {
            stage_data->SetTile(hovered_grid_x, hovered_grid_y, selected_tile_type);
        }

        if (input->GetMouse(MOUSE_INPUT_RIGHT))
        {
            stage_data->SetTile(hovered_grid_x, hovered_grid_y, NONE);
        }
    }

    // --- カメラ移動（UI操作中は無効） ---
    if (!ui_handled)
    {
        float speed = 8.0f;
        if (input->GetKey(KEY_INPUT_W)) camera_offset.y -= speed;
        if (input->GetKey(KEY_INPUT_S)) camera_offset.y += speed;
        if (input->GetKey(KEY_INPUT_A)) camera_offset.x -= speed;
        if (input->GetKey(KEY_INPUT_D)) camera_offset.x += speed;
    }
}

void StageEditor::Draw(Vector2D)
{
    Cursor c = InputManager::GetInstance()->GetMouseCursor();

    // グリッド描画
    for (int y = 0; y < grid_height; ++y)
    {
        for (int x = 0; x < grid_width; ++x)
        {
            eObjectType type = static_cast<eObjectType>(stage_data->GetTile(x, y));
            auto info = GetTypeInfo(type);

            int draw_x0 = x * grid_size - static_cast<int>(camera_offset.x);
            int draw_y0 = y * grid_size - static_cast<int>(camera_offset.y);
            int draw_x1 = (x + 1) * grid_size - static_cast<int>(camera_offset.x);
            int draw_y1 = (y + 1) * grid_size - static_cast<int>(camera_offset.y);

            DrawBox(draw_x0, draw_y0, draw_x1, draw_y1, info.color, TRUE);
            DrawString(draw_x0 + 4, draw_y0 + 4, info.name, GetColor(0, 0, 0));
        }
    }

    // UI描画
    DrawBox(ui_pos.x, ui_pos.y, ui_pos.x + ui_size.x, ui_pos.y + 20, GetColor(80, 80, 80), TRUE);
    DrawString(ui_pos.x + 6, ui_pos.y + 4, "Stage Editor", GetColor(255, 255, 255));

    SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
    DrawBox(selection_box.x, selection_box.y,
        selection_box.x + selection_box.width,
        selection_box.y + selection_box.height,
        GetColor(50, 50, 50), TRUE);
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
    DrawBox(selection_box.x, selection_box.y,
        selection_box.x + selection_box.width,
        selection_box.y + selection_box.height,
        GetColor(0, 0, 0), FALSE);

    // タイル選択ボタン描画
    int y = selection_box.y;
    int button_width = selection_box.width - 10;
    int button_height = 40;
    for (size_t i = 0; i < tile_types.size(); ++i)
    {
        auto info = GetTypeInfo(tile_types[i]);
        int color = (selected_tile_type == tile_types[i]) ? GetColor(255, 255, 0) : info.color;

        SetDrawBlendMode(DX_BLENDMODE_ALPHA, 200);
        DrawBox(selection_box.x + 5, y, selection_box.x + 5 + button_width, y + button_height, color, TRUE);
        SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
        DrawBox(selection_box.x + 5, y, selection_box.x + 5 + button_width, y + button_height, GetColor(0, 0, 0), FALSE);
        DrawFormatString(selection_box.x + 10, y + 10, GetColor(255, 255, 255), "%s", info.name);
        y += 50;
    }
}

void StageEditor::Finalize()
{
}