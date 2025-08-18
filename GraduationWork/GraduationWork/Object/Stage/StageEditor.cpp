#include "StageEditor.h"
#include <DxLib.h>

#include "../../Utility/InputManager.h"

StageEditor::StageEditor(int _grid_size, int _width, int _height)
	: grid_size(_grid_size), width(_width), height(_height)
{
	grid_width = width / grid_size;   
	grid_height = height / grid_size;  
}

StageEditor::~StageEditor()
{
}

void StageEditor::Initialize()
{
	// 初期化処理
}

void StageEditor::Update()
{
	// 更新処理
	InputManager* input = InputManager::GetInstance();

	// マウスクリックでタイル設置
	if(input->GetMouseDown(MOUSE_INPUT_LEFT))
	{
		Cursor c = input->GetMouseCursor();
		int grid_x = c.x / grid_size;
		int grid_y = c.y / grid_size;

		if(grid_x >= 0 && grid_x < grid_width &&
		   grid_y >= 0 && grid_y < grid_height)
		{

		}
	}

	// 右クリックで削除
	if(input->GetMouseDown(MOUSE_INPUT_RIGHT))
	{
		Cursor c = input->GetMouseCursor();
		int grid_x = c.x / grid_size;
		int grid_y = c.y / grid_size;

		if(grid_x >= 0 && grid_x < grid_width &&
		   grid_y >= 0 && grid_y < grid_height)
		{
		}
	}
}

void StageEditor::Draw(Vector2D offset)
{
	for (int i = 0; i <= grid_width; ++i) {
		int x = i * grid_size;
		// グリッドの縦線を描画
		DrawLine(x, 0, x, height, GetColor(200, 200, 200));
	}
	for (int j = 0; j <= grid_height; ++j) {
		int y = j * grid_size;
		// グリッドの横線を描画
		DrawLine(0, y, width, y, GetColor(200, 200, 200));
	}
}

void StageEditor::Finalize()
{
	// 終了処理
}

void StageEditor::UpdateGridSize()
{
	grid_width = width / grid_size;   
	grid_height = height / grid_size;
}
