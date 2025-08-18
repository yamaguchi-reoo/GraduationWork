#pragma once
#include "../../Utility/Vector2D.h"

class StageEditor
{
private:	
	int grid_size; // グリッドのサイズ
	int width;     // ステージの幅
	int height;    // ステージの高さ

	int grid_width;  // グリッドの幅
	int grid_height; // グリッドの高さ

public:
	StageEditor(int _size, int _width, int _height);;
	~StageEditor();

	void Initialize();
	void Update();
	void Draw(Vector2D offset);
	void Finalize();

	void UpdateGridSize();
};

