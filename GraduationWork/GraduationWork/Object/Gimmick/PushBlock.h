#pragma once
#include "../GameObject.h"
#include "../../Utility/Vector2D.h"

class PushBlock :
    public GameObject
{
private:

	bool is_moving = false; // �ړ������ǂ���
	Vector2D move_direction = { 0.0f,0.0f };
	Vector2D move_speed = 1.5f; // �ړ����x
	float move_progress = 0.0f; // �ړ��̐i���i0.0f ���� 1.0f�j
	float move_distance = 12.0f; // �ړ�����

	Vector2D current_move_dir = { 0, 0 };

	const float gravity = 0.5f;       // �d�͉����x
	const float terminal_velocity = 8.0f; // �ő嗎�����x
	bool on_ground = false;         // �n�ʂɐڂ��Ă��邩
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

public:
	void StartMove(Vector2D dir); // �ړ��J�n����
	void MoveUpdate();            // �ړ�����

};

