#pragma once
#include "../GameObject.h"
#include "../../Utility/UserTemplate.h"

#include <vector>

class CharacterBase :
	public GameObject
{
protected:
	bool on_ground = false;           //�n�ʂɂ��邩�ǂ���

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

