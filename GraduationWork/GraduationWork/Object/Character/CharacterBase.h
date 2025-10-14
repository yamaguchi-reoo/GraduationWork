#pragma once
#include "../GameObject.h"
#include "../../Utility/UserTemplate.h"

#include <vector>

class CharacterBase :
	public GameObject
{
protected:
	int hp = 0;                // �q�b�g�|�C���g�i�C�ӂ̒l�Œ����\�j

	bool on_ground = false;           //�n�ʂɂ��邩�ǂ���
	bool is_jumping = false;


	Vector2D velocity = { 0, 0 };     // �ړ����x�ix, y�j
	const float gravity = 0.5f;       // �d�͉����x�i�C�ӂ̒l�Œ����\�j
	const float max_fall_speed = 10.0f; // �ő嗎�����x

	//std::vector<int> animation_data;//�A�j���[�V�����f�[�^
public:
	//����������
	virtual void Initialize(Vector2D _location, Vector2D _box_size) override;
	//�X�V����
	virtual void Update()override;
	//�`�揈��
	virtual void Draw(Vector2D offset, double rate)  override;
	//�I��������
	virtual void Finalize()override;

	virtual void OnHitCollision(GameObject* hit_object)override;

	//�e�̃v���C���[���ǂ���
	virtual bool IsPlayerShadow() const { return false; }

	bool IsOnGround() const { return on_ground; }

};

