#include "GoalPoint.h"
#include "../Character/Player/Player.h"

void GoalPoint::Initialize(Vector2D _location, Vector2D _box_size)
{
	object_type = GOALPOINT;
	__super::Initialize(_location, _box_size);
}

void GoalPoint::Update()
{
	__super::Update();
}

void GoalPoint::Draw(Vector2D offset, double rate)
{
	Vector2D screen_pos = location - offset;
	__super::Draw(screen_pos, rate);
	// デバッグ用に当たり判定を描画
}

void GoalPoint::Finalize()
{
	__super::Finalize();
}

void GoalPoint::OnHitCollision(GameObject* hit_object)
{
	int type = hit_object->GetObjectType();
	if (type == PLAYER)
	{
		reached = true; // ゴールに到達したことを記録
	}
}
