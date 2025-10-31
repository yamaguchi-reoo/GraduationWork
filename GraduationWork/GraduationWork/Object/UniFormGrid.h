#pragma once
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <cmath>

class GameObject;
struct Vector2D;

// 矩形領域（整数座標）
struct IntRect
{
	int x0, y0, x1, y1;
	IntRect() : x0(0), y0(0), x1(0), y1(0) {}
	IntRect(int _x0, int _y0, int _x1, int _y1) : x0(_x0), y0(_y0), x1(_x1), y1(_y1) {}
};

class UniFormGrid
{
private:
	// グリッド全体のワールド座標範囲
	float world_x0;
	float world_y0;
	float world_x1;
	float world_y1;

	// 各セルの一辺の長さ
	float cell_size;

	// セルの数（横方向・縦方向）
	int cols;
	int rows;

	// 各セルに属するオブジェクトのリスト
	std::vector<std::vector<GameObject*>> cells;

	// オブジェクトがどのセルに属しているかのマップ
	std::unordered_map<GameObject*, std::vector<size_t>> object_cells;

public:
	UniFormGrid();

	// グリッドの初期化
	void Initialize(float w_x0, float w_y0, float w_x1, float w_y1, float c_size);

	// 全データをクリア
	void Clear();

	// オブジェクトをグリッドに登録
	void InsertObject(GameObject* obj);
	// オブジェクトをグリッドから削除
	void RemoveObject(GameObject* obj);
	// オブジェクトの位置更新
	void UpdateObject(GameObject* obj);

	// 指定矩形領域に含まれるオブジェクトを取得
	void QueryArea(float min_x, float min_y, float max_x, float max_y, std::vector<GameObject*>& out);

	// 指定セル範囲に含まれるオブジェクトを取得
	void QueryCellRange(int cx0, int cy0, int cx1, int cy1, std::vector<GameObject*>& out);

public:
	// ワールド座標をセル座標に変換
	int WorldToCellX(float wx) const;
	int WorldToCellY(float wy) const;

	// セル座標を一次元インデックスに変換
	size_t CellIndex(int cx, int cy) const;

	// セル座標が有効範囲内かどうか
	bool ValidCell(int cx, int cy) const;

	int GetCols() const { return cols; }
	int GetRows() const { return rows; }
	int GetCellSize() const { return static_cast<int>(cell_size); }

private:
	// オブジェクトのAABB（当たり範囲）からカバーするセル範囲を算出する
	void ComputeCoveredCells(float minx, float miny, float maxx, float maxy, int& out_cx0, int& out_cy0, int& out_cx1, int& out_cy1);

};

