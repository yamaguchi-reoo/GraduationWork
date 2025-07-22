#pragma once
#include "../../SceneBase.h"
#include "../../../common.h"

#include "../../../Object/ObjectInfo.h"


class GameMainScene :
    public SceneBase
{
private:
	int stage_width_num;    //�X�e�[�W�̉��̃u���b�N��
	int stage_height_num;   //�X�e�[�W�̏c�̃u���b�N��
	int stage_data[STAGE_MAX_HEIGHT][STAGE_MAX_WIDTH];

	GameObject* player;	//�v���C���[�̃|�C���^
public:
	GameMainScene();
	~GameMainScene();

	void Initialize() override;		//����������
	eSceneType Update() override;	//�X�V����
	void Draw() override;		//�`�揈��
	void Finalize() override;		//�I��������

	eSceneType GetNowSceneType()const override;

	//�X�e�[�W�f�[�^��ǂݍ���
	void LoadStage();

	//�X�e�[�W�f�[�^�𐶐�
	void SetStage();

	//�J�����̈ʒu���X�V
	void UpdateCamera();

	//�v���C���[��T��
	void FindPlayer();
};

