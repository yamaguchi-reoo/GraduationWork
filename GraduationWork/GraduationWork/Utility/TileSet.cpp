#include "TileSet.h"
#include <DxLib.h>
#include <iostream>

TileSet::TileSet(const std::string& file_path, int tile_width, int tile_height)
	:tile_width(tile_width), tile_height(tile_height)
{
	sprite_handle = LoadGraph(file_path.c_str());
	if (sprite_handle == -1) {
		std::cerr << "タイルセット画像の読み込みに失敗: " << file_path << std::endl;
		return;
	}
}

TileSet::~TileSet()
{
	DeleteGraph(sprite_handle);
}

void TileSet::AddTile(int id, int sprite_x, int sprite_y, bool is_collision)
{
	tile_table[id] = { id, sprite_x, sprite_y, is_collision };
}

void TileSet::DrawTile(int tile_id, int x, int y) const
{
	auto it = tile_table.find(tile_id);
	if (it == tile_table.end()) return;

	const TileInfo& info = it->second;
	DrawRectGraph(x, y, info.sprite_x * tile_width, info.sprite_y * tile_height, tile_width, tile_height, sprite_handle, TRUE);

}

const TileInfo* TileSet::GetTileInfo(int id) const
{
	auto it = tile_table.find(id);
	if (it == tile_table.end()) return nullptr;

	return &it->second;
}
