#pragma once
#include "../../Utility/Vector2D.h"
#include "StageData.h"
#include "../ObjectType.h"
#include <vector>

struct SelectionBox {
	int x, y;
	int width, height;
	bool is_dragging;
	Vector2D offset; // ドラッグ時のオフセット

	SelectionBox()
		: x(300), y(100), width(120), height(80),
		is_dragging(false), offset{ 0 } {
	}
};


class StageEditor
{
private:
	StageData* stage_data; // ステージデータ

	int grid_size; // グリッドのサイズ
	int width;     // ステージの幅（タイル数）
	int height;    // ステージの高さ（タイル数）

	int grid_width;  // グリッドの幅（タイル数）
	int grid_height; // グリッドの高さ（タイル数）

	eObjectType selected_tile_type; // 選択中のタイルタイプ
	std::vector<eObjectType> tile_types; // タイルタイプのリスト

	// UI ドラッグ関連
	Vector2D ui_pos;			// UI の左上座標 (スクリーン座標)
	Vector2D ui_size;			// UI のサイズ
	bool ui_dragging;			// UI をドラッグ中か
	Vector2D ui_drag_offset;	// ドラッグ時のマウスと UI の差分
	SelectionBox selection_box;

	Vector2D camera_offset; // カメラオフセット

	int hovered_grid_x = -1;
	int hovered_grid_y = -1;
public:
	StageEditor(int _grid_size, StageData* _stage_data);
	~StageEditor();

	void Initialize();
	// offset はカメラオフセット（world -> screen 用）
	void Update(Vector2D& offset);
	void Draw(Vector2D offset);
	void Finalize();

	void MoveCamera(float dx, float dy) { camera_offset.x += dx; camera_offset.y += dy; }
	const Vector2D& GetCameraOffset() const { return camera_offset; }

private:
	// UI サイズを計算するヘルパー
	void CalcUIRect(int& out_w, int& out_h) const;

	// UI を画面内に収める
	void ClampUIToScreen();
};
