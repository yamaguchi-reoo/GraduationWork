#include "CharacterBase.h"

void CharacterBase::Initialize(Vector2D _location, Vector2D _box_size)
{
	__super::Initialize(_location, _box_size);
	on_ground = false;
}

void CharacterBase::Update()
{
    on_ground = false;

    // 地面にいないときだけ落下処理
    if (!on_ground)
    {
        velocity.y += gravity;

        // 最大落下速度を超えないように制限
        if (velocity.y > max_fall_speed)
            velocity.y = max_fall_speed;
    }

    // 位置の更新
    location.y += velocity.y;
	location.x += velocity.x; 

    __super::Update();
}

void CharacterBase::Draw(Vector2D offset, double rate)
{
	__super::Draw(offset, rate);
}

void CharacterBase::Finalize()
{
	__super::Finalize();
}

void CharacterBase::OnHitCollision(GameObject* hit_object)
{
	// ブロックとのみ処理
	if (hit_object->GetObjectType() != BLOCK) return;

	// AABB情報取得（座標は左上基準と仮定）
	Vector2D my_pos = GetLocation();
	Vector2D my_size = GetBoxSize();
	Vector2D other_pos = hit_object->GetLocation();
	Vector2D other_size = hit_object->GetBoxSize();

	// 各辺
	float my_bottom = my_pos.y + my_size.y;
	float my_top = my_pos.y;
	float my_left = my_pos.x;
	float my_right = my_pos.x + my_size.x;

	float other_bottom = other_pos.y + other_size.y;
	float other_top = other_pos.y;
	float other_left = other_pos.x;
	float other_right = other_pos.x + other_size.x;

	// 各方向からの押し出し量を計算
	float from_top = other_bottom - my_top;
	float from_bottom = my_bottom - other_top;
	float from_left = other_right - my_left;
	float from_right = my_right - other_left;

	// 最小の押し出し方向を判定
	float min_y = std::min(from_top, from_bottom);
	float min_x = std::min(from_left, from_right);

	if (min_y < min_x) {
		// 垂直方向の衝突
		if (from_top < from_bottom) {
			// ブロックの上に着地
			location.y = other_bottom;
			velocity.y = 0;
		}
		else {
			// 地面に着地
			location.y = other_top - my_size.y;
			velocity.y = 0;
			on_ground = true;
		}
	}
	else {
		// 水平方向の衝突
		if (from_left < from_right) {
			// 左からぶつかった
			location.x = other_right;
			velocity.x = 0;
		}
		else {
			// 右からぶつかった
			location.x = other_left - my_size.x;
			velocity.x = 0;
		}
	}
}
