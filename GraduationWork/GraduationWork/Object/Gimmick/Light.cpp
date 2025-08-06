#include "Light.h"
#include "DxLib.h"
#include <cmath>

void Light::Initialize(Vector2D _location, Vector2D _box_size)
{
	object_type = LIGHT;
	__super::Initialize(_location, _box_size);

    pivot = { _location.x + _box_size.x / 2, _location.y }; // 柱の上端中央

	length = _box_size.y;
	angle = 0.0f;
	angular_speed = 0.02f;
}

void Light::Update()
{
	__super::Update();

	angle += angular_speed;
	if (angle > 0.52f || angle < -0.52f) angular_speed *= -1;

	// 支点から柱の中心位置を計算（柱の長さの半分の位置）
	float halfLength = length / 2.0f;

	location.x = pivot.x + halfLength * sin(angle) - box_size.x / 2;
	location.y = pivot.y + halfLength * cos(angle) - box_size.y / 2;
}

void Light::Draw(Vector2D offset, double rate)
{
	__super::Draw(offset, rate);

#ifdef _DEBUG
		DrawBoxAA(offset.x, offset.y, offset.x + box_size.x, offset.y + box_size.y, GetColor(255, 255, 0), TRUE);
#endif // _DEBUG
}

void Light::Finalize()
{
	__super::Finalize();
}
void Light::OnHitCollision(GameObject* hit_object)
{
}