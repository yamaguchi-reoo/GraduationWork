#include "TileSet.h"
#include <DxLib.h>
#include <iostream>

#include "../Object/ObjectType.h"

#include <nlohmann/json.hpp>
#include <fstream>
#include "tinyxml2.h"  
using namespace tinyxml2;


TileSet::TileSet(const std::string& file_path, int tile_width, int tile_height)
	:tile_width(tile_width), tile_height(tile_height)
{
	sprite_handle = LoadGraph(file_path.c_str());
	if (sprite_handle == -1)
	{
		std::cerr << "Failed to load image: " << file_path << std::endl;
		image_width = 0;
		image_height = 0;
	}
	else
	{
		// 画像の幅・高さを取得
		GetGraphSize(sprite_handle, &image_width, &image_height);
	}
}

TileSet::~TileSet()
{
	Unload();
}

void TileSet::AddTile(int id, int sprite_x, int sprite_y, bool is_collision)
{
	TileInfo info;
	info.id = id;
	info.sprite_x = sprite_x;
	info.sprite_y = sprite_y;
	info.width = tile_width;  
	info.height = tile_height; 
	info.is_collision = is_collision;

	tile_table[id] = info;
}


void TileSet::DrawTile(int tile_id, int x, int y) const
{
	auto it = tile_table.find(tile_id);
	if (it == tile_table.end()) return;

	const TileInfo& info = it->second;
	DrawRectGraph(
		x, y,
		info.sprite_x, info.sprite_y,
		info.width, info.height,
		sprite_handle,
		TRUE
	);
}


const TileInfo* TileSet::GetTileInfo(int id) const
{
	auto it = tile_table.find(id);
	if (it == tile_table.end()) return nullptr;

	return &it->second;
}

void TileSet::LoadAllTiles(bool default_collision)
{
	if (sprite_handle == -1 || image_width == 0 || image_height == 0) return;

	int tiles_x = image_width / tile_width;
	int tiles_y = image_height / tile_height;
	int id = BLOCK;

	for (int y = 0; y < tiles_y; ++y)
	{
		for (int x = 0; x < tiles_x; ++x)
		{
			AddTile(id, x, y, default_collision);
			++id;
		}
	}
}

void TileSet::DrawAllTiles(int start_x, int start_y, int tiles_per_row) const
{
	int index = 0;
	for (const auto& pair : tile_table)
	{
		int id = pair.first;

		int col = index % tiles_per_row;
		int row = index / tiles_per_row;

		int draw_x = start_x + col * tile_width;
		int draw_y = start_y + row * tile_height;

		DrawTile(id, draw_x, draw_y);

		// IDを描画（任意）
		//DrawFormatString(draw_x, draw_y + tile_height - 12, GetColor(255, 255, 255), "%d", id);

		++index;
	}
}

void TileSet::LoadFromJson(const std::string& json_path)
{
	using json = nlohmann::json;
	std::ifstream file(json_path);
	json data;
	file >> data;

	int id = 0;

	for (const auto& frame : data["frames"])
	{
		std::string filename = frame["filename"];
		auto rect = frame["frame"];

		TileInfo info;
		info.id = id;
		info.sprite_x = rect["x"];
		info.sprite_y = rect["y"];
		info.width = rect["w"];
		info.height = rect["h"];
		info.is_collision = false;

		tile_table[id] = info;
		id++;
	}
}

void TileSet::LoadFromXML(const std::string& xml_path)
{
	tinyxml2::XMLDocument doc;
	if (doc.LoadFile(xml_path.c_str()) != XML_SUCCESS) {
		std::cerr << "Failed to load XML file: " << xml_path << std::endl;
		return;
	}

	XMLElement* root = doc.FirstChildElement("TextureAtlas");
	if (!root) return;

	int id = 0;
	for (XMLElement* sub = root->FirstChildElement("SubTexture"); sub; sub = sub->NextSiblingElement("SubTexture"))
	{
		int x = sub->IntAttribute("x");
		int y = sub->IntAttribute("y");
		int w = sub->IntAttribute("width");
		int h = sub->IntAttribute("height");


		AddTile(id, x, y, false);
		++id;
	}
}

void TileSet::DrawSelectedTiles(const std::vector<int>& tile_ids, int start_x, int start_y, int tiles_per_row) const
{
	for (size_t i = 0; i < tile_ids.size(); ++i) {
		int tile_id = tile_ids[i];

		// タイルが存在するかチェック
		if (tile_table.find(tile_id) == tile_table.end()) continue;

		int col = i % tiles_per_row;
		int row = i / tiles_per_row;
		int draw_x = start_x + col * tile_width;
		int draw_y = start_y + row * tile_height;

		DrawTile(tile_id, draw_x, draw_y);

		// タイルIDを表示（デバッグ用）
		DrawFormatString(draw_x, draw_y + tile_height - 12, GetColor(255, 255, 0), "%d", tile_id);
	}
}

void TileSet::DrawTileRange(int start_id, int end_id, int start_x, int start_y, int tiles_per_row) const
{
	std::vector<int> tile_ids;
	for (int id = start_id; id <= end_id; ++id) {
		if (tile_table.find(id) != tile_table.end()) {
			tile_ids.push_back(id);
		}
	}
	DrawSelectedTiles(tile_ids, start_x, start_y, tiles_per_row);
}

int TileSet::GetTileCount() const
{
	return (int)tile_table.size();
}

bool TileSet::HasTile(int tile_id) const
{
	return tile_table.find(tile_id) != tile_table.end();
}

void TileSet::Unload()
{
	if (sprite_handle != -1) {
		DeleteGraph(sprite_handle);
		sprite_handle = -1;
	}
	tile_table.clear(); 
}

