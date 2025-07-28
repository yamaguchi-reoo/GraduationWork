#include "InGameScene.h"
#include "DxLib.h"

#include "../../../Object/ObjectList.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include <string>


InGameScene::InGameScene() :stage_width_num(0), stage_height_num(0), stage_data(),
tile_set("Resource/Images/Tiles/tile.png", BLOCK_SIZE, BLOCK_SIZE)
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
}

eSceneType InGameScene::Update( )
{
	object_manager.Update();
	UpdateCamera(); 

	return __super::Update();
}

void InGameScene::Draw()
{
	//DrawBox(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, GetColor(255, 255, 255), TRUE); // �w�i�������h��Ԃ�
	// �`�揈��
	__super::Draw();
	object_manager.Draw(camera_location, 1.0);

	/*std::vector<int> favorite_tiles = { 0, 5, 12, 25, 31, 45, 62, 78, 89, 105 };
	tile_set.DrawSelectedTiles(favorite_tiles, 10, 20, 5);*/


	DrawString(200, 0, "GameMain", GetColor(255, 255, 255));


	//DrawFormatString(10, 90, GetColor(255,255,255), "Camera Location: (%3f, %3f)", camera_location.x, camera_location.y);

	
}

void InGameScene::Finalize()
{
	// �I��������
	__super::Finalize();
	object_manager.Finalize();
}

eSceneType InGameScene::GetNowSceneType() const
{
	return eSceneType::GAME_MAIN;
}

void InGameScene::LoadStage()
{
	std::ifstream file("Resource/File/Stage.csv");
	if (!file)
	{
		std::cerr << "�X�e�[�W�t�@�C�����J���܂���ł���: " << std::endl;
		return;
	}

	// 1�s�ڂ�ǂݍ���ŃX�e�[�W���ƍ������擾
	std::string line;

	//file����1�s�ǂݍ���ŁAline �Ɋi�[�B
	if (std::getline(file, line)) {
		//���������͂��邽�߂̃X�g���[�����쐬�B
		std::stringstream ss(line);
		std::string width, height;

		// �J���}�ŕ������ĕ��ƍ������擾
		std::getline(ss, width, ',');
		std::getline(ss, height, ',');

		//������𐮐��ɕϊ�
		stage_width_num = std::stoi(width);   // �X�e�[�W��
		stage_height_num = std::stoi(height); // �X�e�[�W����
	}

	stage_data.resize(stage_height_num);
	for (int i = 0; i < stage_height_num; ++i) {
		stage_data[i].resize(stage_width_num);
	}

	//�X�e�[�W�f�[�^�̓ǂݍ��݁iCSV��2�s�ڈȍ~�j
	for (int i = 0; i < stage_height_num; i++) {
		//1�s���ǂݍ���
		if (std::getline(file, line)) {
			std::stringstream ss(line);
			for (int j = 0; j < stage_width_num; j++) {
				//�J���}��؂�Ńf�[�^���擾
				std::string value;
				if (std::getline(ss, value, ',')) {
					//������𐮐��ɕϊ����ăX�e�[�W�f�[�^�Ɋi�[
					stage_data[i][j] = std::stoi(value);
				}
			}
		}
	}

	file.close();

	SetStage();

}

void InGameScene::SetStage()
{
	//1�u���b�N�̑傫��
	const Vector2D block_size((float)BLOCK_SIZE);

	for (int y = 0; y < stage_height_num; ++y)
	{
		for (int x = 0; x < stage_width_num; ++x)
		{
			int tile = stage_data[y][x];

			// ���[���h���W�̌v�Z�i�������_��Y���W�𔽓]���Ă����j
			Vector2D world_pos(x * BLOCK_SIZE, SCREEN_HEIGHT - ((stage_height_num - y) * BLOCK_SIZE));

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
			}
		}
	}

}

void InGameScene::UpdateCamera()
{
	GameObject* player = object_manager.FindObjectType(eObjectType::PLAYER);
	if (!player) return;

	float screen_half_width = SCREEN_WIDTH / 2.0f;
	float stage_limit_left = 0.0f;
	float stage_limit_right = static_cast<float>(100) * BLOCK_SIZE - SCREEN_WIDTH;

	camera_location.x = player->GetLocation().x - screen_half_width;

	if (camera_location.x < stage_limit_left) camera_location.x = stage_limit_left;
	if (camera_location.x > stage_limit_right) camera_location.x = stage_limit_right;
}

