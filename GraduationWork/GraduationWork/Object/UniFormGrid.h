#pragma once
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <cmath>

class GameObject;
struct Vector2D;

// ��`�̈�i�������W�j
struct IntRect
{
	int x0, y0, x1, y1;
	IntRect() : x0(0), y0(0), x1(0), y1(0) {}
	IntRect(int _x0, int _y0, int _x1, int _y1) : x0(_x0), y0(_y0), x1(_x1), y1(_y1) {}
};

class UniFormGrid
{
private:
	// �O���b�h�S�̂̃��[���h���W�͈�
	float world_x0;
	float world_y0;
	float world_x1;
	float world_y1;

	// �e�Z���̈�ӂ̒���
	float cell_size;

	// �Z���̐��i�������E�c�����j
	int cols;
	int rows;

	// �e�Z���ɑ�����I�u�W�F�N�g�̃��X�g
	std::vector<std::vector<GameObject*>> cells;

	// �I�u�W�F�N�g���ǂ̃Z���ɑ����Ă��邩�̃}�b�v
	std::unordered_map<GameObject*, std::vector<size_t>> object_cells;

public:
	UniFormGrid();

	// �O���b�h�̏�����
	void Initialize(float w_x0, float w_y0, float w_x1, float w_y1, float c_size);

	// �S�f�[�^���N���A
	void Clear();

	// �I�u�W�F�N�g���O���b�h�ɓo�^
	void InsertObject(GameObject* obj);
	// �I�u�W�F�N�g���O���b�h����폜
	void RemoveObject(GameObject* obj);
	// �I�u�W�F�N�g�̈ʒu�X�V
	void UpdateObject(GameObject* obj);

	// �w���`�̈�Ɋ܂܂��I�u�W�F�N�g���擾
	void QueryArea(float min_x, float min_y, float max_x, float max_y, std::vector<GameObject*>& out);

	// �w��Z���͈͂Ɋ܂܂��I�u�W�F�N�g���擾
	void QueryCellRange(int cx0, int cy0, int cx1, int cy1, std::vector<GameObject*>& out);

public:
	// ���[���h���W���Z�����W�ɕϊ�
	int WorldToCellX(float wx) const;
	int WorldToCellY(float wy) const;

	// �Z�����W���ꎟ���C���f�b�N�X�ɕϊ�
	size_t CellIndex(int cx, int cy) const;

	// �Z�����W���L���͈͓����ǂ���
	bool ValidCell(int cx, int cy) const;

	int GetCols() const { return cols; }
	int GetRows() const { return rows; }
	int GetCellSize() const { return static_cast<int>(cell_size); }

private:
	// �I�u�W�F�N�g��AABB�i������͈́j����J�o�[����Z���͈͂��Z�o����
	void ComputeCoveredCells(float minx, float miny, float maxx, float maxy, int& out_cx0, int& out_cy0, int& out_cx1, int& out_cy1);

};

