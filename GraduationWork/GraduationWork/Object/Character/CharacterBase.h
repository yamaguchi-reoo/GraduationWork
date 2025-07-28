#pragma once
#include "../GameObject.h"
#include "../../Utility/UserTemplate.h"

#include <vector>

class CharacterBase :
	public GameObject
{
protected:
	bool on_ground = false;           //�n�ʂɂ��邩�ǂ���

	Vector2D velocity = { 0, 0 };     // �ړ����x�ix, y�j
	const float gravity = 0.5f;       // �d�͉����x�i�C�ӂ̒l�Œ����\�j
	const float max_fall_speed = 10.0f; // �ő嗎�����x

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

	bool IsOnGround() const { return on_ground; }

};

