#pragma once
#include <vector>
#include <string>
#include "../../Utility/TileSet.h"
#include "../../Utility/Vector2D.h"

struct PlacedTile
{
	int tile_id;      // タイル番号
	Vector2D pos;     // ワールド座標（任意）
	float scale;      // 拡大縮小（オプション）
	bool active;
};

class StageData
{
private:
	int width;       // ステージの幅
	int height;      // ステージの高さ

	std::vector<std::vector<int>> tile_layer;    // 見た目用タイル
	std::vector<std::vector<int>> object_layer;  // ゲームオブジェクト

	std::vector<PlacedTile> free_tiles; // 自由配置タイル用
public:
	StageData(int _width, int _height);

	void Resize(int _width, int _height);  // ステージサイズ変更
	void Clear(int value = 0);  // 初期化

	int GetTile(int x, int y) const;
	void SetTile(int x, int y, int id);

	int GetObj(int x, int y);
	void SetObj(int x, int y, int id);

	// ファイル入出力
	bool LoadCSV(const std::string& filename);
	bool SaveCSV(const std::string& filename);

	bool LoadTileCSV(const std::string& filename);
	bool SaveTileCSV(const std::string& filename);

	bool LoadCSVInternal(const std::string& filename, std::vector<std::vector<int>>& layer);
	bool SaveCSVInternal(const std::string& filename, const std::vector<std::vector<int>>& layer);

	int GetWidth() const { return width; }
	int GetHeight() const { return height; }

public:
	//自由配置タイル用
	void AddPlacedTile(int id, const Vector2D& pos);
	void RemovePlacedTileNear(const Vector2D& pos, float radius);

	bool SaveFreeTilesCSV(const std::string& filename);
	bool LoadFreeTilesCSV(const std::string& filename);

	const std::vector<PlacedTile>& GetFreeTiles() const { return free_tiles; }

};

