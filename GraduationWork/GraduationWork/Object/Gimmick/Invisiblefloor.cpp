#include "Invisiblefloor.h"
#include "DxLib.h"

void Invisiblefloor::Initialize(Vector2D _location, Vector2D _box_size)
{
	object_type = INVISIBLEFLOOR;
	__super::Initialize(_location, _box_size);
}

void Invisiblefloor::Update()
{
	__super::Update();
}

void Invisiblefloor::Draw(Vector2D offset, double rate)
{
	Vector2D screen_pos = location - offset;
	__super::Draw(screen_pos, rate);
#ifdef _DEBUG
	DrawBoxAA(screen_pos.x, screen_pos.y, screen_pos.x + box_size.x, screen_pos.y + box_size.y, GetColor(0, 255, 255), TRUE);
#endif // _DEBUG
}

void Invisiblefloor::Finalize()
{
	__super::Finalize();
}

void Invisiblefloor::OnHitCollision(GameObject* hit_object)
{
}
