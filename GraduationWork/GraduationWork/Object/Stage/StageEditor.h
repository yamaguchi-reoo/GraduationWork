#pragma once
#include "../../Utility/Vector2D.h"
#include "StageData.h"

class StageEditor
{
private:	
	StageData* stage_data; // ステージデータ

	int grid_size; // グリッドのサイズ
	int width;     // ステージの幅
	int height;    // ステージの高さ

	int grid_width;  // グリッドの幅
	int grid_height; // グリッドの高さ

public:
	StageEditor(int _size, StageData* _stage_data);
	~StageEditor();

	void Initialize();
	void Update(Vector2D offset);
	void Draw(Vector2D offset);
	void Finalize();

	void UpdateGridSize();
};

