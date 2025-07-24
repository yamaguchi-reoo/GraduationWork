#pragma once
#include "../../SceneBase.h"
#include "../../../common.h"

#include "../../../Object/ObjectInfo.h"
#include "../../../Object/ObjectManager.h"


class InGameScene :
    public SceneBase
{
private:
	int stage_width_num;    //�X�e�[�W�̉��̃u���b�N��
	int stage_height_num;   //�X�e�[�W�̏c�̃u���b�N��

	std::vector<std::vector<int>> stage_data; //�X�e�[�W�f�[�^���i�[����2�����x�N�^�[

	ObjectManager object_manager; //�I�u�W�F�N�g�Ǘ��N���X
public:
	InGameScene();
	~InGameScene();

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
};

