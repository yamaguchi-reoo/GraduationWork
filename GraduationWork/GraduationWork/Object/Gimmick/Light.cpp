#include "Light.h"
#include "DxLib.h"

void Light::Initialize(Vector2D _location, Vector2D _box_size)
{
	object_type = LIGHT;
	__super::Initialize(_location, _box_size);
}

void Light::Update()
{
	__super::Update();
}

void Light::Draw(Vector2D offset, double rate)
{
	Vector2D screen_pos = location - offset;
	__super::Draw(screen_pos, rate);
#ifdef _DEBUG
		DrawBoxAA(screen_pos.x, screen_pos.y, screen_pos.x + box_size.x, screen_pos.y + box_size.y, GetColor(255, 255, 0), TRUE);
#endif // _DEBUG
}

void Light::Finalize()
{
	__super::Finalize();
}
void Light::OnHitCollision(GameObject* hit_object)
{
}