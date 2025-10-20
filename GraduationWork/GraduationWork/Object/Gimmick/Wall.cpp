#include "Wall.h"

void Wall::Initialize(Vector2D _location, Vector2D _box_size)
{
	object_type = WALL;

	__super::Initialize(_location, _box_size);
}

void Wall::Update()
{
	__super::Update();
}

void Wall::Draw(Vector2D offset, double rate)
{


	Vector2D screen_pos = location - offset;

#ifdef _DEBUG
	DrawBoxAA(screen_pos.x, screen_pos.y, screen_pos.x + box_size.x, screen_pos.y + box_size.y, GetColor(0, 125, 125), TRUE);
#endif
	__super::Draw(screen_pos, rate);


}


void Wall::Finalize()
{
	__super::Finalize();
}

void Wall::OnHitCollision(GameObject* hit_object)
{
}
