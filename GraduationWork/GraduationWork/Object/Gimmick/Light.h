#pragma once
#include "../GameObject.h"

struct LightData {
	Vector2D origin = { 0.0f,0.0f };      // ���_�i�Œ�j
	float angle = 0.0f;          // ���݂̌���
	float min_angle = 0.0f;      // �ŏ��p�x
	float max_angle = 0.0f;      // �ő�p�x
	float rotate_speed = 0.0f;   // ��]���x�i�x/�b�j
	bool moving = false;         // �����łœ��쒆���ǂ���
	int direction = 1;			 // 1: ��������], -1: �t������]
};

class Light :
    public GameObject
{
private:
	Vector2D apex = { 0.0f };
	float width = 0.0f;
	float height = 0.0f;
	float max_height = 0.0f;

	LightData light_data;

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

private:
	bool PointInTriangle(const Vector2D& p, const Vector2D& a, const Vector2D& b, const Vector2D& c);

	bool CheckLightCollision(GameObject* obj);
	
};

