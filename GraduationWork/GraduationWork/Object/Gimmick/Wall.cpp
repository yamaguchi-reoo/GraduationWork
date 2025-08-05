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
	__super::Draw(offset, rate);
}


void Wall::Finalize()
{
	__super::Finalize();
}

void Wall::OnHitCollision(GameObject* hit_object)
{
}
