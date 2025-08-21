#include "InGameScene.h"
#include <DxLib.h>

#include <fstream>
#include <sstream>
#include <iostream>
#include <string>

#include "../../../Object/ObjectList.h"
#include "../../../Utility/InputManager.h"


InGameScene::InGameScene() :stage_width_num(0), stage_height_num(0), stage_data(0, 0),
tile_set("Resource/Images/Tiles/tile.png", BLOCK_SIZE, BLOCK_SIZE),editor(nullptr),
edit_mode(false)
{
	// JSON����^�C���Z�b�g��ǂݍ���
	tile_set.LoadFromJson("Resource/Images/Tiles/tile.json"); 
}

InGameScene::~InGameScene()
{
}

void InGameScene::Initialize()
{
	camera_location = Vector2D(0.0f, 0.0f); // �J�����̏����ʒu��ݒ�
	object_manager.Initialize(); // �I�u�W�F�N�g�Ǘ��N���X�̏�����

	// ����������
	LoadStage();

	// �X�e�[�W�G�f�B�^�[�̏�����
	editor = new StageEditor(BLOCK_SIZE, &stage_data);
}

eSceneType InGameScene::Update()
{
	InputManager* input = InputManager::GetInstance();

#ifdef _DEBUG
	// F1�ŕҏW���[�h�؂�ւ�
	if (input->GetKeyDown(KEY_INPUT_F1))
	{
		if (edit_mode)
		{
			// �ҏW���[�h�I�� �� �ۑ����čēǂݍ���
			//editor->SaveStageData("stage.csv");
			stage_data.SaveCSV("Resource/File/Stage.csv");

			// �I�u�W�F�N�g����U�N���A
			object_manager.Finalize();    // �I�u�W�F�N�g�������
			object_manager.Initialize();  // ������

			// �X�e�[�W�f�[�^�ǂݍ���
			LoadStage();
		}

		edit_mode = !edit_mode;
	}
#endif

	if (edit_mode)
	{
		// �ҏW���[�h���F�G�f�B�^�[�����X�V
		editor->Update(camera_location);
	}
	else
	{
		// �ʏ탂�[�h�F�I�u�W�F�N�g���X�V
		object_manager.Update(camera_location);
	}

	// �J�����͗����[�h�ōX�V
	UpdateCamera();

	return __super::Update();
}

void InGameScene::Draw()
{
	DrawBox(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, GetColor(255, 0, 0), FALSE); 
	// �ʏ�`��
	__super::Draw();
	object_manager.Draw(camera_location, 1.0);

	if (edit_mode)
	{
		// �w�i�𔼓����ō����h��
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128); // 128 = 50%����
		DrawBox(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, GetColor(0, 0, 0), TRUE);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0); // �u�����h����

		// �G�f�B�^�[�̃O���b�h�`��
		editor->Draw(camera_location);

		// �ҏW���[�h�\��
		DrawString(600, 10, "EDIT MODE", GetColor(255, 255, 0));
	}
	else
	{
		DrawString(600, 10, "GAME MODE", GetColor(255, 255, 255));
	}

	/*std::vector<int> favorite_tiles = { 0, 5, 12, 25, 31, 45, 62, 78, 89, 105 };
	tile_set.DrawSelectedTiles(favorite_tiles, 10, 20, 5);*/	
}

void InGameScene::Finalize()
{
	// �I��������
	__super::Finalize();
	object_manager.Finalize();
	editor->Finalize();
}

eSceneType InGameScene::GetNowSceneType() const
{
	return eSceneType::GAME_MAIN;
}

void InGameScene::LoadStage()
{
	if (!stage_data.LoadCSV("Resource/File/Stage.csv"))
	{
		std::cerr << "�X�e�[�W�t�@�C�����J���܂���ł���\n";
		return;
	}

	SetStage(); // �f�[�^�����ƂɃI�u�W�F�N�g����
}

void InGameScene::SetStage()
{
	const Vector2D block_size((float)BLOCK_SIZE);

	for (int y = 0; y < stage_data.GetHeight(); ++y)
	{
		for (int x = 0; x < stage_data.GetWidth(); ++x)
		{
			int tile = stage_data.GetTile(x, y);

			// ���㌴�_�ŕ`��i�������y�����j
			Vector2D world_pos(x * BLOCK_SIZE, y * BLOCK_SIZE);

			switch (tile)
			{
			case NONE:
				break;
			case BLOCK:
				object_manager.CreateObject<Block>(world_pos, block_size);
				break;
			case PLAYER:
				object_manager.CreateObject<Player>(world_pos, Vector2D(48.0f, 64.0f));
				break;
			case WALL:
				object_manager.CreateObject<Wall>(world_pos, Vector2D(48.0f, 48.0f));
				break;
			case LIGHT:
				object_manager.CreateObject<Light>(world_pos, Vector2D(170.0f, 70.0f));
				break;
			case INVISIBLEFLOOR:
				object_manager.CreateObject<Invisiblefloor>(world_pos, Vector2D(96.0f, 14.0f));
				break;
			case PUSHBLOCK:
				object_manager.CreateObject<PushBlock>(world_pos, Vector2D(48.0f, 48.0f));
				break;
			case PLATE:
				object_manager.CreateObject<Plate>(world_pos, Vector2D(100.0f,10.0f));
				break;
			}
		}
	}
}

void InGameScene::UpdateCamera()
{
	if (edit_mode)
	{
		// �ҏW���[�h�ł� editor �̃J�������g�p
		camera_location = editor->GetCameraOffset();
	}
	else
	{
		GameObject* player = object_manager.FindObjectType(eObjectType::PLAYER);
		if (!player) return;

		float screen_half_width = SCREEN_WIDTH / 2.0f;
		float stage_limit_left = 0.0f;
		float stage_limit_right = static_cast<float>(stage_data.GetWidth()) * BLOCK_SIZE - SCREEN_WIDTH;

		camera_location.x = player->GetLocation().x - screen_half_width;

		if (camera_location.x < stage_limit_left) camera_location.x = stage_limit_left;
		if (camera_location.x > stage_limit_right) camera_location.x = stage_limit_right;
	}
}
