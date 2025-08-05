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
	__super::Draw(offset, rate);
#ifdef _DEBUG
		DrawBoxAA(offset.x, offset.y, offset.x + box_size.x, offset.y + box_size.y, GetColor(0, 255, 255), TRUE);
#endif // _DEBUG
}

void Light::Finalize()
{
	__super::Finalize();
}
void Light::OnHitCollision(GameObject* hit_object)
{
}