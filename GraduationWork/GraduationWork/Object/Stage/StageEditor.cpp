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

    // �^�C���Z�b�g�������i����̃p�X���g���B�K�v�Ȃ���������Ă��������j
    /*tile_set = new TileSet("Resource/Images/Tiles/tile.png", BLOCK_SIZE, BLOCK_SIZE);
    tile_set->LoadFromJson("Resource/Images/Tiles/tile.json");*/

    tile_set = new TileSet("Resource/Images/Tiles/tiles_spritesheet.png", BLOCK_SIZE, BLOCK_SIZE);
    tile_set->LoadFromXML("Resource/Images/Tiles/tiles_spritesheet.xml");

    // tile_ids �����
    int count = tile_set->GetTileCount();
    tile_ids.reserve(count);
    for (int i = 0; i < count; ++i) tile_ids.push_back(i);

    // UI�p�l�����E�����v�Z�i�]�����݁j
    ui_panel_width = tiles_per_row * tile_width + 20;
    ui_panel_height = SCREEN_HEIGHT - 20;

    // UI�͉E���ɌŒ�
    ui_pos.x = static_cast<float>(SCREEN_WIDTH) - ui_panel_width;
    ui_pos.y = 0;
    ui_size.x = static_cast<float>(ui_panel_width);
    ui_size.y = static_cast<float>(ui_panel_height + 20); // �w�b�_���܂�

    selection_box.x = static_cast<int>(ui_pos.x);
    selection_box.y = 20; // �w�b�_��
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
    // offset �͌Ăяo�����̃J�����ʒu�iworld->screen �p�j�Ȃ̂œ������čŌ�ɏ����߂�
    camera_offset = offset;

    InputManager* input = InputManager::GetInstance();
    Cursor c = input->GetMouseCursor();
    Vector2D mouse_pos{ (float)c.x, (float)c.y };

    bool ui_handled = false;

    // �� ���[�h�؂�ւ�
    if (input->GetKeyDown(KEY_INPUT_TAB))
    {
        current_mode = (current_mode == EditMode::Tile) ? EditMode::Object : EditMode::Tile;
    }

    // UI ���͏����i�X�N���[��/�I���j
    HandleUIInput(mouse_pos, ui_handled);

    // �O���b�h�ҏW�iUI ���삪�D�悳���j
    HandleGridEditing(mouse_pos, ui_handled);

    // �J�����ړ��iUI �̊O�ł̂݁j
    HandleCameraMovement(!ui_handled);

    // �����߂��i�O���� camera offset ���X�V�j

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
    else { // Object���[�h
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
        preview_tile.active = false;
        return;
    }

    InputManager* input = InputManager::GetInstance();

    if (hovered_grid_x >= 0 && hovered_grid_y >= 0) {
        Vector2D grid_pos{
            static_cast<float>(hovered_grid_x * grid_size),
            static_cast<float>(hovered_grid_y * grid_size)
        };

        if (current_mode == EditMode::Tile && selected_tile_id >= 0) {
            // ���N���b�N�Ŕz�u
            if (input->GetMouse(MOUSE_INPUT_LEFT)) {
                stage_data->AddPlacedTile(selected_tile_id, grid_pos);
            }
            // �E�N���b�N�ō폜
            if (input->GetMouse(MOUSE_INPUT_RIGHT)) { 
                stage_data->RemovePlacedTileNear(grid_pos, tile_set->GetTileWidth() / 2.0f); 
            }

            // �v���r���[
            preview_tile.tile_id = selected_tile_id;
            preview_tile.pos = grid_pos;
            preview_tile.active = true;
        }
        else if (current_mode == EditMode::Object) {
            // ���N���b�N�ŃI�u�W�F�N�g�z�u
            if (input->GetMouse(MOUSE_INPUT_LEFT)) {
                stage_data->SetObj(hovered_grid_x, hovered_grid_y, static_cast<int>(selected_object_type));
            }
            // �E�N���b�N�ō폜
            if (input->GetMouse(MOUSE_INPUT_RIGHT)) {
                stage_data->SetObj(hovered_grid_x, hovered_grid_y, 0);
            }
        }
    }
    else {
        preview_tile.active = false;
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
    // ���R�z�u���Ȃ�O���b�h��\��
    if (current_mode == EditMode::Tile)
    {
        for (int y = 0; y < grid_height; ++y)
        {
            for (int x = 0; x < grid_width; ++x)
            {
                int draw_x = x * grid_size - static_cast<int>(camera_offset.x);
                int draw_y = y * grid_size - static_cast<int>(camera_offset.y);

                int id = stage_data->GetTile(x, y);
                if (id >= 0 && tile_set->HasTile(id))
                {
                    tile_set->DrawTile(id, draw_x, draw_y);
                }

                // ID�\���͔C��
                if (id == -1) continue;
                DrawFormatString(draw_x, draw_y, GetColor(255, 255, 0), "%d", id);
            }
        }
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
        // �z�o�[�\���͎c��
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
    // �w�b�_�[
    const int header_h = 20;
    DrawBox(static_cast<int>(ui_pos.x), static_cast<int>(ui_pos.y),
        static_cast<int>(ui_pos.x) + selection_box.width,
        static_cast<int>(ui_pos.y) + header_h, GetColor(80, 80, 80), TRUE);
    DrawString(static_cast<int>(ui_pos.x) + 6, static_cast<int>(ui_pos.y) + 4, "Stage Editor", GetColor(255, 255, 255));

    const char* mode_text = (current_mode == EditMode::Tile) ? "Mode:TILE" : "Mode:OBJ";
    DrawFormatString((int)ui_pos.x + 6, (int)ui_pos.y + 4, GetColor(255, 255, 255),
        "Stage Editor [%s]", mode_text);

    // �w�i�i���j
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, 200);
    DrawBox(selection_box.x, selection_box.y,
        selection_box.x + selection_box.width,
        selection_box.y + selection_box.height,
        GetColor(255, 255, 255), TRUE);
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

    // �g��
    DrawBox(selection_box.x, selection_box.y,
        selection_box.x + selection_box.width,
        selection_box.y + selection_box.height,
        GetColor(0, 0, 0), FALSE);
}

void StageEditor::DrawTiles()
{
    if (current_mode == EditMode::Tile) {
        // --- �}�b�v���̃^�C���� DrawGrid() �ɔC���� (��ŕ`��ς�) ---

        // �^�C�����X�g�`��i�E���p�l���j
        int y_start = selection_box.y - ui_scroll_offset;
        int button_w = tile_width - 5;
        int button_h = tile_height - 5;

        InputManager* input = InputManager::GetInstance();
        Cursor c = input->GetMouseCursor();

        for (size_t i = 0; i < tile_ids.size(); ++i) {
            int row = static_cast<int>(i) / tiles_per_row;
            int col = static_cast<int>(i) % tiles_per_row;
            int x = selection_box.x + col * tile_width + 5;
            int y = y_start + row * tile_height;

            if (y + button_h < selection_box.y || y > selection_box.y + selection_box.height)
                continue;

            // �w�i�E�I��\��
            bool selected = (selected_tile_id == tile_ids[i]);
            int bg = selected ? GetColor(255, 255, 0) : GetColor(255, 255, 255);

            // �z�o�[���o�iUI�p�l�����̃}�E�X�j
            bool hovered = (c.x >= x && c.x <= x + button_w && c.y >= y && c.y <= y + button_h);
            if (hovered && !selected) bg = GetColor(180, 220, 255);

            SetDrawBlendMode(DX_BLENDMODE_ALPHA, 200);
            DrawBox(x, y, x + button_w, y + button_h, bg, TRUE);
            SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
            DrawBox(x, y, x + button_w, y + button_h, GetColor(0, 0, 0), FALSE);

            // tile_ids �� tile_set �̃C���f�b�N�X�i-1 ���܂܂Ȃ��͂������O�̂��߃`�F�b�N�j
            int tid = tile_ids[i];
            if (tid < 0) continue;

            // �^�C���摜���{�^�����ɕ`��
            TileInfo const* info = tile_set->GetTileInfo(tid);
            if (info) {
                tile_set->DrawTile(tid, x + 5, y + 5);
            }

            // ID�\���͔C�ӁB���� -1 �̂Ƃ��͕\�����Ȃ��i���ɃX�L�b�v�ς݁j
            DrawFormatString(x + button_w - 24, y + button_h - 16, GetColor(50, 50, 50), "%d", tid);
        }

        // �I�𒆃^�C���̑傫�߃v���r���[�i�p�l���㕔�j
        if (selected_tile_id >= 0) {
            int preview_x = selection_box.x + 8;
            int preview_y = selection_box.y + 6;
            int preview_size = Min(tile_width - 16, tile_height - 16);
            DrawBox(preview_x - 4, preview_y - 4, preview_x + preview_size + 4, preview_y + preview_size + 4,
                GetColor(255, 255, 255), TRUE);
            DrawBox(preview_x - 4, preview_y - 4, preview_x + preview_size + 4, preview_y + preview_size + 4,
                GetColor(0, 0, 0), FALSE);
            tile_set->DrawTile(selected_tile_id, preview_x, preview_y);
            DrawFormatString(preview_x + preview_size + 8, preview_y + 8, GetColor(0, 0, 0), "ID: %d", selected_tile_id);
        }
    }
    else {
        // Object���[�h�̊����`��i���̂܂܁j
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
    // �m��ς݃^�C���`��
    for (const auto& tile : stage_data->GetFreeTiles())
    {
        int draw_x = static_cast<int>(tile.pos.x - camera_offset.x);
        int draw_y = static_cast<int>(tile.pos.y - camera_offset.y);
        tile_set->DrawTile(tile.tile_id, draw_x, draw_y);
    }

    // �v���r���[�`��
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
