#pragma once
#include "../GameObject.h"
class Light :
    public GameObject
{
private:
	
	Vector2D pivot;
	float angle;
	float length;
	float Draw_length;
	int beam_width;
	
	float target_angle;  // �ڕW�p�x
	float angle_speed;   // �p�x�̕ω����x

	Vector2D corner[4];
	
	// �����蔻��p�i�n�ʕt�߂ɉ����Œu�������j
// �Ⴆ�� pivot ��菭�����iy�����ցj�ɉ����̍L����`��ݒ�
	Vector2D hit_box_pos;   // �����蔻���`�̍�����W
	Vector2D hit_box_size;  // �����蔻���`�̕��E����

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

	void DrawLightColumn(Vector2D pivot, float width, float length, float angle_rad);

	void DrawParallelogramFilled(Vector2D p0, Vector2D p1, Vector2D p2, Vector2D p3, unsigned int color);
};

