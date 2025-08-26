#include <DxLib.h>
#include "Enemy.h"
#include "../../ObjectManager.h"
#include "../../../Utility/UtilityList.h"
#include "../../../common.h"

Enemy::Enemy():
    max_hp(3),
    damage(1),
    is_dead(false),
    is_alive(true)
{
}

Enemy::~Enemy()
{
}

void Enemy::Initialize(Vector2D _location, Vector2D _box_size)
{
    __super::Initialize(_location, _box_size);

    object_type = ENEMY;

    hp = max_hp;
   
}

void Enemy::Update()
{
    // 簡易パトロール移動（例：左右に移動）
    velocity.x = 1.5f; // 必要なら向きを制御

    __super::Update();

    if (hp <= 0)
    {
        is_dead = true;
        is_alive = false;
    }
}

void Enemy::Draw(Vector2D offset, double rate)
{
    Vector2D screen_pos = location - offset;

    __super::Draw(offset, rate);

    if (is_dead) return;

    // 敵キャラ本体の描画
    DrawBoxAA(
        screen_pos.x, screen_pos.y, screen_pos.x + box_size.x, screen_pos.y + box_size.y,
        GetColor(0, 255, 0), // 緑色の敵
        TRUE
    );

#ifdef _DEBUG
    DrawFormatStringF(location.x + offset.x, location.y + offset.y - 16, GetColor(255, 255, 255), "Enemy HP: %d", hp);
#endif
}

void Enemy::Finalize()
{
    __super::Finalize();
}

void Enemy::OnHitCollision(GameObject* hit_object)
{
    __super::OnHitCollision(hit_object);

    int type = hit_object->GetObjectType();

    if (type == WALL || type == BLOCK)
    {
        // 簡単な方向転換ロジック追加可
        velocity.x *= -1;
    }
}

void Enemy::ReceiveDamage(int dmg)
{
    hp -= dmg;
    if (hp <= 0)
    {
        hp = 0;
        is_dead = true;
    }
}
