#include "PushBlock.h"
#include "DxLib.h"

#include "../Character/Player/Player.h"
#include "../../Utility/UserTemplate.h"


void PushBlock::Initialize(Vector2D _location, Vector2D _box_size)
{
	object_type = PUSHBLOCK;
	__super::Initialize(_location, _box_size);

	on_ground = false;
}

void PushBlock::Update()
{
	on_ground = false;

	// 地面にいないときだけ落下処理
	if (!on_ground)
	{
		velocity.y += gravity;

		// 最大落下速度を超えないように制限
		if (velocity.y > terminal_velocity)
			velocity.y = terminal_velocity;
	}

	// 位置の更新
	location.y += velocity.y;
	location.x += velocity.x;

	// 横押しの移動
	if (is_moving) {
		MoveUpdate();
	}

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
    int type = hit_object->GetObjectType();
    if (type != BLOCK && type != WALL && type != PUSHBLOCK && type != PLAYER) return;

    // プレイヤーが影状態ならPushBlockの衝突を無視してすり抜ける
    if (type == PLAYER)
    {
        Player* player = dynamic_cast<Player*>(hit_object);
        if (player && player->IsPlayerShadow())
        {
            return; // すり抜ける
        }
    }

    Vector2D my_pos = GetLocation();
    Vector2D my_size = GetBoxSize();
    Vector2D other_pos = hit_object->GetLocation();
    Vector2D other_size = hit_object->GetBoxSize();

    float my_bottom = my_pos.y + my_size.y;
    float my_top = my_pos.y;
    float my_left = my_pos.x;
    float my_right = my_pos.x + my_size.x;

    float other_bottom = other_pos.y + other_size.y;
    float other_top = other_pos.y;
    float other_left = other_pos.x;
    float other_right = other_pos.x + other_size.x;

    float from_top = other_bottom - my_top;
    float from_bottom = my_bottom - other_top;
    float from_left = other_right - my_left;
    float from_right = my_right - other_left;

    float min_y = Min(from_top, from_bottom);
    float min_x = Min(from_left, from_right);

    bool vertical_collision = (min_y < min_x);
    bool horizontal_collision = !vertical_collision;

    // 押し出し処理（位置調整）
    if (vertical_collision) {
        if (from_top < from_bottom) {
            location.y = other_bottom;
            velocity.y = 0;
        }
        else {
            location.y = other_top - my_size.y;
            velocity.y = 0;
            on_ground = true;
        }
    }
    else {
        if (from_left < from_right) {
            location.x = other_right;
        }
        else {
            location.x = other_left - my_size.x;
        }
    }
}

void PushBlock::StartMove(Vector2D dir)
{
    move_direction = dir;
	is_moving = true;
	move_progress = 0.0f;
}

void PushBlock::MoveUpdate()
{
	float move_step = move_speed.x; 
	location += move_direction * move_step;
	move_progress += move_step;

	if (move_progress >= move_distance)
	{
		is_moving = false;
		move_progress = 0.0f;
	}
}

