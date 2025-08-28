#pragma once
#include <vector>
#include <string>
#include "../../Utility/TileSet.h"
#include "../../Utility/Vector2D.h"

struct PlacedTile
{
	int tile_id;      // �^�C���ԍ�
	Vector2D pos;     // ���[���h���W�i�C�Ӂj
	float scale;      // �g��k���i�I�v�V�����j
	bool active;
};

class StageData
{
private:
	int width;       // �X�e�[�W�̕�
	int height;      // �X�e�[�W�̍���

	std::vector<std::vector<int>> tile_layer;    // �����ڗp�^�C��
	std::vector<std::vector<int>> object_layer;  // �Q�[���I�u�W�F�N�g

	std::vector<PlacedTile> free_tiles; // ���R�z�u�^�C���p
public:
	StageData(int _width, int _height);

	void Resize(int _width, int _height);  // �X�e�[�W�T�C�Y�ύX
	void Clear(int value = 0);  // ������

	int GetTile(int x, int y) const;
	void SetTile(int x, int y, int id);

	int GetObj(int x, int y);
	void SetObj(int x, int y, int id);

	// �t�@�C�����o��
	bool LoadCSV(const std::string& filename);
	bool SaveCSV(const std::string& filename);

	bool LoadTileCSV(const std::string& filename);
	bool SaveTileCSV(const std::string& filename);

	bool LoadCSVInternal(const std::string& filename, std::vector<std::vector<int>>& layer);
	bool SaveCSVInternal(const std::string& filename, const std::vector<std::vector<int>>& layer);

	int GetWidth() const { return width; }
	int GetHeight() const { return height; }

public:
	//���R�z�u�^�C���p
	void AddPlacedTile(int id, const Vector2D& pos);
	void RemovePlacedTileNear(const Vector2D& pos, float radius);

	bool SaveFreeTilesCSV(const std::string& filename);
	bool LoadFreeTilesCSV(const std::string& filename);

	const std::vector<PlacedTile>& GetFreeTiles() const { return free_tiles; }

};

