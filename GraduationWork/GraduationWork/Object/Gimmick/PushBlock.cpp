#include "PushBlock.h"
#include "DxLib.h"


void PushBlock::Initialize(Vector2D _location, Vector2D _box_size)
{
	object_type = PUSHBLOCK;
	__super::Initialize(_location, _box_size);
}

void PushBlock::Update()
{
	__super::Update();
}

void PushBlock::Draw(Vector2D offset, double rate)
{
	Vector2D screen_pos = location - offset;
	__super::Draw(screen_pos, rate);
#ifdef _DEBUG
	DrawBoxAA(screen_pos.x, screen_pos.y, screen_pos.x + box_size.x, screen_pos.y + box_size.y, GetColor(0, 255, 255), TRUE);
#endif // _DEBUG
}

void PushBlock::Finalize()
{
	__super::Finalize();
}

void PushBlock::OnHitCollision(GameObject* hit_object)
{
}