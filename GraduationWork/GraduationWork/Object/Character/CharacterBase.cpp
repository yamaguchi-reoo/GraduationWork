#include "CharacterBase.h"
#include "../Gimmick/PushBlock.h"

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
	Vector2D screen_pos = location - offset;
	__super::Draw(screen_pos, rate);
}

void CharacterBase::Finalize()
{
	__super::Finalize();
}

void CharacterBase::OnHitCollision(GameObject* hit_object)
{
    int type = hit_object->GetObjectType();

    // 影状態なら「壁」も「押せるブロック」もすり抜ける
    if (IsPlayerShadow() && (type == WALL || type == PUSHBLOCK)) return;

    // 対象はブロック or 壁
    if (type != BLOCK && type != WALL && type != PUSHBLOCK && type != INVISIBLEFLOOR) return;

    // AABB情報取得（左上基準）
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

    // 各方向からの押し出し量
    float from_top = other_bottom - my_top;
    float from_bottom = my_bottom - other_top;
    float from_left = other_right - my_left;
    float from_right = my_right - other_left;

    float min_y = from_top < from_bottom ? from_top : from_bottom;
    float min_x = from_left < from_right ? from_left : from_right;

    // 優先方向を決める（速度が大きい方向を優先）
    bool vertical_first = (fabsf(velocity.y) > fabsf(velocity.x) + 0.1f);

    // 微小オフセット
    const float epsilon = 0.01f;

    if (vertical_first && min_y < min_x)
    {
        // 垂直優先
        if (from_top < from_bottom)
        {
            // 上から押し返される
            location.y = other_bottom + epsilon;
            if (velocity.y < 0) velocity.y = 0;
        }
        else
        {
            // 下から着地
            location.y = other_top - my_size.y;
            velocity.y = 0;
            on_ground = true;
            is_jumping = false;
        }

        // ほんの少し水平をずらす
        if (velocity.x > 0) location.x -= epsilon;
        else if (velocity.x < 0) location.x += epsilon;
    }
    else if (!vertical_first && min_x < min_y)
    {
        // 水平優先
        if (from_left < from_right)
        {
            // 左から押し返される
            location.x = other_right;
            velocity.x = 0;

            if (type == PUSHBLOCK)
            {
                PushBlock* pb = dynamic_cast<PushBlock*>(hit_object);
                if (pb) pb->StartMove(Vector2D(-1.0f, 0.0f));
            }
        }
        else
        {
            // 右から押し返される
            location.x = other_left - my_size.x;
            velocity.x = 0;

            if (type == PUSHBLOCK)
            {
                PushBlock* pb = dynamic_cast<PushBlock*>(hit_object);
                if (pb) pb->StartMove(Vector2D(1.0f, 0.0f));
            }
        }

        // ほんの少し垂直をずらす
        if (velocity.y > 0) location.y -= epsilon;
        else if (velocity.y < 0) location.y += epsilon;
    }
    else
    {
        // どちらでも微調整で重なりを解消
        if (min_y < min_x)
        {
            if (from_top < from_bottom)
                location.y = other_bottom + epsilon;
            else
            {
                location.y = other_top - my_size.y;
                velocity.y = 0;
                on_ground = true;
                is_jumping = false;
            }
        }
        else
        {
            if (from_left < from_right)
                location.x = other_right;
            else
                location.x = other_left - my_size.x;

            velocity.x = 0;
        }
    }
}

