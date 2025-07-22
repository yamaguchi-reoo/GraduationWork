#include "InGameScene.h"
#include "DxLib.h"

#include <fstream>
#include <sstream>
#include <iostream>

GameMainScene::GameMainScene()
{
}

GameMainScene::~GameMainScene() 
{
}

void GameMainScene::Initialize()
{
	// ����������
	LoadStage();
}

eSceneType GameMainScene::Update()
{
	// �X�V����
	UpdateCamera();
	FindPlayer();
	return __super::Update();
}

void GameMainScene::Draw()
{
	// �`�揈��
	__super::Draw();
}

void GameMainScene::Finalize()
{
	// �I��������
	__super::Finalize();
}

eSceneType GameMainScene::GetNowSceneType() const
{
	return eSceneType::GAME_MAIN;
}

void GameMainScene::LoadStage()
{
}

void GameMainScene::SetStage()
{
}

void GameMainScene::UpdateCamera()
{
	//�v���C���[�����݂���Ȃ�J������Ǐ]������
	if (player)
	{
		float screen_half_width = SCREEN_WIDTH / 2;				//��ʂ̔����̕�
		float stage_limit_left = 0.0f;							//�X�e�[�W�̍��[
		float stage_limit_right = stage_width_num * BLOCK_SIZE - SCREEN_WIDTH; //�X�e�[�W�̉E�[ 

		//�J�����ʒu �� �v���C���[�̈ʒu - ��ʂ̔����̕� 
		camera_location.x = player->GetLocation().x - screen_half_width;

		//��ʒ[�ł̓X�N���[�����Ȃ��悤����
		if (camera_location.x < stage_limit_left) camera_location.x = stage_limit_left;
		if (camera_location.x > stage_limit_right) camera_location.x = stage_limit_right;
	}
}

void GameMainScene::FindPlayer()
{
	//�v���C���[�I�u�W�F�N�g��T���Ď擾
	for (auto obj : objects)
	{
		if (obj->GetObjectType() == PLAYER)
		{
			player = obj;
			break;
		}
	}
}
