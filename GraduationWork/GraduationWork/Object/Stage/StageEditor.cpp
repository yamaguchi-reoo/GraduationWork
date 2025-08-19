#include "StageEditor.h"
#include <DxLib.h>

#include "../../Utility/InputManager.h"
#include "../../common.h"

StageEditor::StageEditor(int _grid_size, StageData* _stage_data)
	: grid_size(_grid_size), stage_data(_stage_data)
{
	width = stage_data->GetWidth();
	height = stage_data->GetHeight();

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

void StageEditor::Update(Vector2D offset)
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
	int cols = SCREEN_WIDTH / grid_size + 1;
	int rows = SCREEN_HEIGHT / grid_size + 1;

	for (int i = 0; i <= cols; ++i) {
		int x = i * grid_size - static_cast<int>(offset.x) % grid_size;
		DrawLine(x, 0, x, SCREEN_HEIGHT, GetColor(200, 200, 200));
	}

	for (int j = 0; j <= rows; ++j) {
		int y = j * grid_size - static_cast<int>(offset.y) % grid_size;
		DrawLine(0, y, SCREEN_WIDTH, y, GetColor(200, 200, 200));
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
