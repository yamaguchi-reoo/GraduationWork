#pragma once
#include <unordered_map>
#include <string>

struct TileInfo
{
	int id;
	int sprite_x;
	int sprite_y;
	bool is_collision;
};

class TileSet
{
private:
	std::unordered_map<int, TileInfo> tile_table;
	int sprite_handle;
	int tile_width;
	int tile_height;

public:
	TileSet(const std::string& file_path, int tile_width, int tile_height);;
	~TileSet();

	void AddTile(int tile_id, int sprite_x, int sprite_y, bool is_collision);

	void DrawTile(int id, int x, int y) const;
	const TileInfo* GetTileInfo(int id) const;


};

