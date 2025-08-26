#pragma once

#include "../../Utility/Vector2D.h"
#include "../../Utility/TileSet.h"
#include "StageData.h"
#include "../ObjectType.h"
#include <vector>

enum class EditMode {
    Tile,
    Object
};


struct SelectionBox {
    int x, y;           // ������W
    int width, height;  // �T�C�Y
    bool is_dragging;   // �h���b�O���t���O
    Vector2D offset;    // �h���b�O���̃}�E�X�Ƃ̍���

    SelectionBox()
        : x(300), y(100), width(120), height(80),
        is_dragging(false), offset{ 0 } {
    }
};

class StageEditor
{
private:
    StageData* stage_data; // �ҏW�Ώۂ̃X�e�[�W�f�[�^
    int grid_size;         // �O���b�h�T�C�Y�i1�}�X�̃s�N�Z���j
    int width, height;     // �X�e�[�W�̕��E�����i�^�C�����j
    int grid_width, grid_height; // �O���b�h�T�C�Y�i�^�C���P�ʁj

    EditMode current_mode;

    eObjectType selected_object_type;       // ���ݑI�𒆂̃^�C���^�C�v
    std::vector<eObjectType> object_types;  // �^�C���^�C�v���X�g
    int selected_tile_id;                 // UI�őI�𒆂̃^�C��ID
    std::vector<int> tile_ids;            // UI�p�^�C��ID���X�g


    TileSet* tile_set; // �`��p�^�C���Z�b�g
    Vector2D camera_offset; // �J�����I�t�Z�b�g�i�X�N���[���p�j


    Vector2D ui_pos;        // UI������W
    Vector2D ui_size;       // UI�T�C�Y
    bool ui_dragging;       // UI�h���b�O����
    Vector2D ui_drag_offset;// �h���b�O���̍���
    SelectionBox selection_box;


    int ui_scroll_offset;   // ���݂̃X�N���[����
    int max_scroll;         // �ő�X�N���[����
    int tiles_per_row;      // UI�^�C��1�s������̃^�C����
    int tile_width;         // �^�C���{�^����
    int tile_height;        // �^�C���{�^������
    int ui_panel_width;     // UI�p�l����
    int ui_panel_height;    // UI�p�l������


    int hovered_grid_x = -1;
    int hovered_grid_y = -1;

public:
    StageEditor(int _grid_size, StageData* _stage_data);
    ~StageEditor();

    void Initialize();
    void Update(Vector2D& offset);  // offset�̓J�����I�t�Z�b�g�p
    void Draw(Vector2D offset);
    void Finalize();


    void MoveCamera(float dx, float dy) { camera_offset.x += dx; camera_offset.y += dy; }
    const Vector2D& GetCameraOffset() const { return camera_offset; }


private:

    void HandleUIInput(const Vector2D& mouse_pos, bool& out_ui_handled);
    void HandleGridEditing(const Vector2D& mouse_pos, bool ui_handled);
    void HandleCameraMovement(bool ui_handled);

    void DrawGrid();
    void DrawUI();
    void DrawTiles();
    void DrawScrollBar();


};
