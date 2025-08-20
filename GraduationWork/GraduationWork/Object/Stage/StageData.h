#pragma once
#include <vector>
#include <string>

class StageData
{
private:
	int width;       // ステージの幅
	int height;      // ステージの高さ
	std::vector<std::vector<int>> data; // ステージデータを格納する2次元ベクター

public:
	StageData(int _width, int _height);

	void Resize(int _width, int _height);  // ステージサイズ変更
	void Clear(int value = 0);  // 初期化

	int GetTile(int x, int y) const;
	void SetTile(int x, int y, int id);

	// ファイル入出力
	bool LoadCSV(const std::string& filename);
	bool SaveCSV(const std::string& filename);

	int GetWidth() const { return width; }
	int GetHeight() const { return height; }

};

