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
    int x, y;           // 左上座標
    int width, height;  // サイズ
    bool is_dragging;   // ドラッグ中フラグ
    Vector2D offset;    // ドラッグ時のマウスとの差分

    SelectionBox()
        : x(300), y(100), width(120), height(80),
        is_dragging(false), offset{ 0 } {
    }
};

class StageEditor
{
private:
    StageData* stage_data; // 編集対象のステージデータ
    int grid_size;         // グリッドサイズ（1マスのピクセル）
    int width, height;     // ステージの幅・高さ（タイル数）
    int grid_width, grid_height; // グリッドサイズ（タイル単位）

    EditMode current_mode;

    eObjectType selected_object_type;       // 現在選択中のタイルタイプ
    std::vector<eObjectType> object_types;  // タイルタイプリスト
    int selected_tile_id;                 // UIで選択中のタイルID
    std::vector<int> tile_ids;            // UI用タイルIDリスト


    TileSet* tile_set; // 描画用タイルセット
    Vector2D camera_offset; // カメラオフセット（スクロール用）


    Vector2D ui_pos;        // UI左上座標
    Vector2D ui_size;       // UIサイズ
    bool ui_dragging;       // UIドラッグ中か
    Vector2D ui_drag_offset;// ドラッグ時の差分
    SelectionBox selection_box;


    int ui_scroll_offset;   // 現在のスクロール量
    int max_scroll;         // 最大スクロール量
    int tiles_per_row;      // UIタイル1行あたりのタイル数
    int tile_width;         // タイルボタン幅
    int tile_height;        // タイルボタン高さ
    int ui_panel_width;     // UIパネル幅
    int ui_panel_height;    // UIパネル高さ


    int hovered_grid_x = -1;
    int hovered_grid_y = -1;

public:
    StageEditor(int _grid_size, StageData* _stage_data);
    ~StageEditor();

    void Initialize();
    void Update(Vector2D& offset);  // offsetはカメラオフセット用
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
