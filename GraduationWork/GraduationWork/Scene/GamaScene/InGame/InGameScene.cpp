#include "InGameScene.h"
#include "DxLib.h"

#include <fstream>
#include <sstream>
#include <iostream>

InGameScene::InGameScene() :stage_width_num(0), stage_height_num(0), stage_data(), player(nullptr)
{
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

eSceneType InGameScene::Update()
{
	// �X�V����
	if (!player)
	{
		FindPlayer();
	}

	object_manager.Update();
	UpdateCamera(); 

	return __super::Update();
}

void InGameScene::Draw()
{
	// �`�揈��
	__super::Draw();
	object_manager.Draw(camera_location, 1.0);

	DrawString(0, 0, "GameMain", GetColor(255, 255, 255));
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
}

void InGameScene::SetStage()
{
}

void InGameScene::UpdateCamera()
{
	//�v���C���[�����݂���Ȃ�J������Ǐ]������
	if (player)
	{
		float screen_half_width = SCREEN_WIDTH / 2;				//��ʂ̔����̕�
		float stage_limit_left = 0.0f;							//�X�e�[�W�̍��[
		float stage_limit_right = static_cast<float>(stage_width_num) * BLOCK_SIZE - SCREEN_WIDTH; //�X�e�[�W�̉E�[ 

		//�J�����ʒu �� �v���C���[�̈ʒu - ��ʂ̔����̕� 
		camera_location.x = player->GetLocation().x - screen_half_width;

		//��ʒ[�ł̓X�N���[�����Ȃ��悤����
		if (camera_location.x < stage_limit_left) camera_location.x = stage_limit_left;
		if (camera_location.x > stage_limit_right) camera_location.x = stage_limit_right;
	}
}

void InGameScene::FindPlayer()
{
	player = object_manager.FindObjectType(PLAYER);
}
