#pragma once
#include <unordered_map>
#include <string>
#include "Vector2D.h"

struct TileInfo
{
	int id;
	int sprite_x; // �X�v���C�g���ł�X
	int sprite_y; // �X�v���C�g���ł�Y
	int width;    // �^�C����
	int height;   // �^�C������
	bool is_collision;
};

class TileSet
{
private:
	std::unordered_map<int, TileInfo> tile_table;
	int sprite_handle;
	int tile_width;
	int tile_height;

	int image_width;   // �摜�S�̂̕��i�s�N�Z���j
	int image_height;  // �摜�S�̂̍����i�s�N�Z���j


public:
	TileSet(const std::string& file_path, int tile_width, int tile_height);;
	~TileSet();

	void AddTile(int tile_id, int sprite_x, int sprite_y, bool is_collision);

	void DrawTile(int id, int x, int y) const;
	const TileInfo* GetTileInfo(int id) const;

	// �^�C��1���̃T�C�Y�擾
	int GetTileWidth() const { return tile_width; }
	int GetTileHeight() const { return tile_height; }

	// �ǉ��F�摜�T�C�Y�擾
	int GetImageWidth() const { return image_width; }
	int GetImageHeight() const { return image_height; }

	// �ǉ��F�摜���̑S�^�C���������o�^����֐�
	void LoadAllTiles(bool default_collision = false);

	void DrawAllTiles(int start_x, int start_y, int tiles_per_row) const;

	// JSON�`���̃^�C������ǂݍ��ފ֐�
	void LoadFromJson(const std::string& json_path);

	// XLML�`���̃^�C������ǂݍ��ފ֐�
	void LoadFromXML(const std::string& xml_path);

	// �w�肵���^�C��ID�̔z���`��
	void DrawSelectedTiles(const std::vector<int>& tile_ids, int start_x, int start_y, int tiles_per_row = 10) const;

	// �w�肵���͈͂̃^�C����`��
	void DrawTileRange(int start_id, int end_id, int start_x, int start_y, int tiles_per_row = 10) const;

	// �^�C���������擾
	int GetTileCount() const;

	// �^�C��ID�����݂��邩�`�F�b�N
	bool HasTile(int tile_id) const;
};

