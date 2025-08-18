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
	// ����������
}

void StageEditor::Update()
{
	// �X�V����
	InputManager* input = InputManager::GetInstance();

	// �}�E�X�N���b�N�Ń^�C���ݒu
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

	// �E�N���b�N�ō폜
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
		// �O���b�h�̏c����`��
		DrawLine(x, 0, x, height, GetColor(200, 200, 200));
	}
	for (int j = 0; j <= grid_height; ++j) {
		int y = j * grid_size;
		// �O���b�h�̉�����`��
		DrawLine(0, y, width, y, GetColor(200, 200, 200));
	}
}

void StageEditor::Finalize()
{
	// �I������
}

void StageEditor::UpdateGridSize()
{
	grid_width = width / grid_size;   
	grid_height = height / grid_size;
}
