#pragma once
#include "../../Utility/Vector2D.h"
#include "StageData.h"

class StageEditor
{
private:	
	StageData* stage_data; // �X�e�[�W�f�[�^

	int grid_size; // �O���b�h�̃T�C�Y
	int width;     // �X�e�[�W�̕�
	int height;    // �X�e�[�W�̍���

	int grid_width;  // �O���b�h�̕�
	int grid_height; // �O���b�h�̍���

public:
	StageEditor(int _size, StageData* _stage_data);
	~StageEditor();

	void Initialize();
	void Update(Vector2D offset);
	void Draw(Vector2D offset);
	void Finalize();

	void UpdateGridSize();
};

