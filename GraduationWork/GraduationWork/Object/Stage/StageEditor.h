#pragma once
#include "../../Utility/Vector2D.h"

class StageEditor
{
private:	
	int grid_size; // �O���b�h�̃T�C�Y
	int width;     // �X�e�[�W�̕�
	int height;    // �X�e�[�W�̍���

	int grid_width;  // �O���b�h�̕�
	int grid_height; // �O���b�h�̍���

public:
	StageEditor(int _size, int _width, int _height);;
	~StageEditor();

	void Initialize();
	void Update();
	void Draw(Vector2D offset);
	void Finalize();

	void UpdateGridSize();
};

