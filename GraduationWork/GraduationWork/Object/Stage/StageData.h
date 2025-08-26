#pragma once
#include <vector>
#include <string>

class StageData
{
private:
	int width;       // ステージの幅
	int height;      // ステージの高さ

	std::vector<std::vector<int>> tile_layer;    // 見た目用タイル
	std::vector<std::vector<int>> object_layer;  // ゲームオブジェクト

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

};

