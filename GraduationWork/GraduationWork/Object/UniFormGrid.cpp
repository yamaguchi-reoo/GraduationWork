#include "UniFormGrid.h"
#include "GameObject.h"

UniFormGrid::UniFormGrid() : world_x0(0), world_y0(0), world_x1(0), world_y1(0), cell_size(1), cols(0), rows(0)
{
}

void UniFormGrid::Initialize(float w_x0, float w_y0, float w_x1, float w_y1, float c_size)
{
	this->world_x0 = w_x0;
	this->world_y0 = w_y0;
	this->world_x1 = w_x1;
	this->world_y1 = w_y1;
	this->cell_size = c_size;

	// ワールド全体をセルサイズで割って列・行数を計算
	cols = static_cast<int>(std::ceil((world_x1 - world_x0) / c_size));
	rows = static_cast<int>(std::ceil((world_y1 - world_y0) / c_size));

	// セルの配列を確保
	cells.clear();
	cells.resize(static_cast<size_t>(cols * rows));

	// オブジェクトのセル情報もリセット
	object_cells.clear();
}

void UniFormGrid::Clear()
{
	for (auto& cell : cells)
		cell.clear();
	object_cells.clear();
}

void UniFormGrid::InsertObject(GameObject* obj)
{
	if (!obj) return;
	// オブジェクトのAABBを取得
	Vector2D loc = obj->GetLocation();
	Vector2D size = obj->GetBoxSize();

	float minx = loc.x;
	float miny = loc.y;
	float maxx = loc.x + size.x;
	float maxy = loc.y + size.y;

	// AABBがカバーするセル範囲を算出
	int cx0, cy0, cx1, cy1;
	ComputeCoveredCells(minx, miny, maxx, maxy, cx0, cy0, cx1, cy1);

	std::vector<size_t> indices;

	// 対象範囲内のすべてのセルに登録
	for (int cy = cy0; cy <= cy1; ++cy)
	{
		for (int cx = cx0; cx <= cx1; ++cx)
		{
			if (!ValidCell(cx, cy)) continue;
			size_t idx = CellIndex(cx, cy);
			cells[idx].push_back(obj);
			indices.push_back(idx);
		}
	}

	// オブジェクトが属するセルインデックスを記録
	object_cells[obj] = indices;
}

void UniFormGrid::RemoveObject(GameObject* obj)
{
	if (!obj) return;

	auto it = object_cells.find(obj);
	if (it == object_cells.end()) return;

	// 登録されていた全セルから削除
	for (size_t idx : it->second)
	{
		auto& vec = cells[idx];
		vec.erase(std::remove(vec.begin(), vec.end(), obj), vec.end());
	}

	object_cells.erase(it);
}

void UniFormGrid::UpdateObject(GameObject* obj)
{
	RemoveObject(obj);
	InsertObject(obj);
}

void UniFormGrid::QueryArea(float min_x, float min_y, float max_x, float max_y, std::vector<GameObject*>& out)
{
	int cx0, cy0, cx1, cy1;
	ComputeCoveredCells(min_x, min_y, max_x, max_y, cx0, cy0, cx1, cy1);

	std::unordered_set<GameObject*> unique;

	for (int cy = cy0; cy <= cy1; ++cy)
	{
		for (int cx = cx0; cx <= cx1; ++cx)
		{
			if (!ValidCell(cx, cy)) continue;
			size_t idx = CellIndex(cx, cy);
			for (auto* obj : cells[idx])
			{
				if (unique.insert(obj).second)
					out.push_back(obj);
			}
		}
	}
}

void UniFormGrid::QueryCellRange(int cx0, int cy0, int cx1, int cy1, std::vector<GameObject*>& out)
{
	std::unordered_set<GameObject*> unique;

	for (int cy = cy0; cy <= cy1; ++cy)
	{
		for (int cx = cx0; cx <= cx1; ++cx)
		{
			if (!ValidCell(cx, cy)) continue;
			size_t idx = CellIndex(cx, cy);
			for (auto* obj : cells[idx])
			{
				if (unique.insert(obj).second)
					out.push_back(obj);
			}
		}
	}
}

int UniFormGrid::WorldToCellX(float wx) const
{
	return static_cast<int>((wx - world_x0) / cell_size);
}

int UniFormGrid::WorldToCellY(float wy) const
{
	return static_cast<int>((wy - world_y0) / cell_size);
}

size_t UniFormGrid::CellIndex(int cx, int cy) const
{
	return static_cast<size_t>(cy * cols + cx);
}

bool UniFormGrid::ValidCell(int cx, int cy) const
{
	return (cx >= 0 && cy >= 0 && cx < cols && cy < rows);
}

void UniFormGrid::ComputeCoveredCells(float minx, float miny, float maxx, float maxy, int& out_cx0, int& out_cy0, int& out_cx1, int& out_cy1)
{
	out_cx0 = WorldToCellX(minx);
	out_cy0 = WorldToCellY(miny);
	out_cx1 = WorldToCellX(maxx);
	out_cy1 = WorldToCellY(maxy);
}
