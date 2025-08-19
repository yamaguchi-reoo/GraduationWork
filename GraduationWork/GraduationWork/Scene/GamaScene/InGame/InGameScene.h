#pragma once
#include "../../SceneBase.h"
#include "../../../common.h"

#include "../../../Object/ObjectInfo.h"
#include "../../../Object/ObjectManager.h"

#include "../../../Utility/TileSet.h" 
#include "../../../Object/Stage/StageEditor.h"
#include "../../../Object/Stage/StageData.h"

class InGameScene :
    public SceneBase
{
private:
	int stage_width_num;    //�X�e�[�W�̉��̃u���b�N��
	int stage_height_num;   //�X�e�[�W�̏c�̃u���b�N��

	StageData stage_data;   //�X�e�[�W�f�[�^���i�[����2�����x�N�^�[
	ObjectManager object_manager; //�I�u�W�F�N�g�Ǘ��N���X
	TileSet tile_set; //�^�C���Z�b�g
	StageEditor* editor; //�X�e�[�W�G�f�B�^�[

	bool edit_mode; // �X�e�[�W�G�f�B�b�g���[�h���ǂ���
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

