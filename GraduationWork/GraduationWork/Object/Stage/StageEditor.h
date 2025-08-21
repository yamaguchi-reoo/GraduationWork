#pragma once
#include "../../Utility/Vector2D.h"
#include "StageData.h"
#include "../ObjectType.h"
#include <vector>

struct SelectionBox {
	int x, y;
	int width, height;
	bool is_dragging;
	Vector2D offset; // �h���b�O���̃I�t�Z�b�g

	SelectionBox()
		: x(300), y(100), width(120), height(80),
		is_dragging(false), offset{ 0 } {
	}
};


class StageEditor
{
private:
	StageData* stage_data; // �X�e�[�W�f�[�^

	int grid_size; // �O���b�h�̃T�C�Y
	int width;     // �X�e�[�W�̕��i�^�C�����j
	int height;    // �X�e�[�W�̍����i�^�C�����j

	int grid_width;  // �O���b�h�̕��i�^�C�����j
	int grid_height; // �O���b�h�̍����i�^�C�����j

	eObjectType selected_tile_type; // �I�𒆂̃^�C���^�C�v
	std::vector<eObjectType> tile_types; // �^�C���^�C�v�̃��X�g

	// UI �h���b�O�֘A
	Vector2D ui_pos;			// UI �̍�����W (�X�N���[�����W)
	Vector2D ui_size;			// UI �̃T�C�Y
	bool ui_dragging;			// UI ���h���b�O����
	Vector2D ui_drag_offset;	// �h���b�O���̃}�E�X�� UI �̍���
	SelectionBox selection_box;

	Vector2D camera_offset; // �J�����I�t�Z�b�g

	int hovered_grid_x = -1;
	int hovered_grid_y = -1;
public:
	StageEditor(int _grid_size, StageData* _stage_data);
	~StageEditor();

	void Initialize();
	// offset �̓J�����I�t�Z�b�g�iworld -> screen �p�j
	void Update(Vector2D& offset);
	void Draw(Vector2D offset);
	void Finalize();

	void MoveCamera(float dx, float dy) { camera_offset.x += dx; camera_offset.y += dy; }
	const Vector2D& GetCameraOffset() const { return camera_offset; }

private:
	// UI �T�C�Y���v�Z����w���p�[
	void CalcUIRect(int& out_w, int& out_h) const;

	// UI ����ʓ��Ɏ��߂�
	void ClampUIToScreen();
};
