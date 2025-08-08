#include "Light.h"
#include "DxLib.h"
#include <cmath>

void Light::Initialize(Vector2D _location, Vector2D _box_size)
{
	object_type = LIGHT;
	__super::Initialize(_location, _box_size);

    pivot = { _location.x + _box_size.x / 2, _location.y }; // ���̏�[����

	length = _box_size.y;
	angle = 0.0f;
	angular_speed = 0.02f;
}

void Light::Update()
{
	__super::Update();

	angle += angular_speed;
	if (angle > 0.52f || angle < -0.52f) angular_speed *= -1;

	float halfLength = length / 2.0f;

	// �`��Ɠ����蔻��̒��S�ƂȂ�ʒu�i���̒��S�j
	location.x = pivot.x + halfLength * sin(angle) - box_size.x / 2;
	location.y = pivot.y + halfLength * cos(angle) - box_size.y / 2;

	// �����[�̓����蔻��̂��߂̈ʒu���X�V
	hit_box.x = pivot.x + length * sin(angle) - hit_box.x / 2;
	hit_box.y = pivot.y + length * cos(angle) - hit_box.y / 2;
}

void Light::Draw(Vector2D offset, double rate)
{
	Vector2D screen_pos = location - offset;
	__super::Draw(screen_pos, rate);
	// �x�_�̃X�N���[�����W
	float px = pivot.x - offset.x;
	float py = pivot.y - offset.y;

	// ���̉��[�̈ʒu�ilength������]�����ɉ��΂��j
	float dx = sin(angle) * length;
	float dy = cos(angle) * length;

	float ex = px + dx;
	float ey = py + dy;

	DrawLine(px, py, ex, ey, GetColor(255, 255, 0), 5); // ���߂̐��Œ��\��
#ifdef _DEBUG
		//DrawBoxAA(screen_pos.x, screen_pos.y, screen_pos.x + box_size.x, screen_pos.y + box_size.y, GetColor(255, 255, 0), TRUE);
#endif // _DEBUG
}

void Light::Finalize()
{
	__super::Finalize();
}
void Light::OnHitCollision(GameObject* hit_object)
{
}