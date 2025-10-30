#pragma once
#include <unordered_map>
#include <string>
#include "Vector2D.h"

struct TileInfo
{
	int id;
	int sprite_x; // スプライト内でのX
	int sprite_y; // スプライト内でのY
	int width;    // タイル幅
	int height;   // タイル高さ
	bool is_collision;
};

class TileSet
{
private:
	std::unordered_map<int, TileInfo> tile_table;
	int sprite_handle;
	int tile_width;
	int tile_height;

	int image_width;   // 画像全体の幅（ピクセル）
	int image_height;  // 画像全体の高さ（ピクセル）


public:
	TileSet(const std::string& file_path, int tile_width, int tile_height);;
	~TileSet();

	void AddTile(int tile_id, int sprite_x, int sprite_y, bool is_collision);

	void DrawTile(int id, int x, int y) const;
	const TileInfo* GetTileInfo(int id) const;

	// タイル1枚のサイズ取得
	int GetTileWidth() const { return tile_width; }
	int GetTileHeight() const { return tile_height; }

	// 追加：画像サイズ取得
	int GetImageWidth() const { return image_width; }
	int GetImageHeight() const { return image_height; }

	// 追加：画像内の全タイルを自動登録する関数
	void LoadAllTiles(bool default_collision = false);

	void DrawAllTiles(int start_x, int start_y, int tiles_per_row) const;

	// JSON形式のタイル情報を読み込む関数
	void LoadFromJson(const std::string& json_path);

	// XLML形式のタイル情報を読み込む関数
	void LoadFromXML(const std::string& xml_path);

	// 指定したタイルIDの配列を描画
	void DrawSelectedTiles(const std::vector<int>& tile_ids, int start_x, int start_y, int tiles_per_row = 10) const;

	// 指定した範囲のタイルを描画
	void DrawTileRange(int start_id, int end_id, int start_x, int start_y, int tiles_per_row = 10) const;

	// タイル総数を取得
	int GetTileCount() const;

	// タイルIDが存在するかチェック
	bool HasTile(int tile_id) const;
};

