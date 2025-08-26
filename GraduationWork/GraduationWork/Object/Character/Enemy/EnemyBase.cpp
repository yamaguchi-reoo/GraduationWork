#include <DxLib.h>
#include "EnemyBase.h"
#include "../../ObjectManager.h"
#include "../../../Utility/UtilityList.h"
#include "../../../common.h"


EnemyBase::EnemyBase()
    : max_hp(3), damage(1), is_dead(false),is_alive(true), move_speed(2.0f), moving_right(true), state(EnemyState::Real)
{
}

EnemyBase::~EnemyBase()
{
}

void EnemyBase::Initialize(Vector2D _location, Vector2D _box_size)
{
    __super::Initialize(_location, _box_size);
    hp = max_hp;
    object_type = ENEMY;
    is_alive = true;
}

void EnemyBase::Update()
{
    velocity.x = moving_right ? move_speed : -move_speed;
    __super::Update();

    if (hp <= 0)
    {
        is_dead = true;
        is_alive = false;
    }
}

void EnemyBase::Draw(Vector2D offset, double rate)
{
    Vector2D screen_pos = location - offset;

    __super::Draw(offset, rate);

    int color = (state == EnemyState::Real) ? GetColor(0, 255, 0) : GetColor(180, 80, 255);

    DrawBoxAA(
        screen_pos.x, screen_pos.y, screen_pos.x + box_size.x, screen_pos.y + box_size.y,
        color, TRUE
    );
}

void EnemyBase::OnHitCollision(GameObject* hit_object)
{
    __super::OnHitCollision(hit_object);

    int type = hit_object->GetObjectType();
    if (type == BLOCK || type == WALL)
    {
        moving_right = !moving_right;
    }
}

void EnemyBase::SwitchState()
{
    if (state == EnemyState::Real)
        state = EnemyState::Shadow;
    else
        state = EnemyState::Real;
}

void EnemyBase::ReceiveDamage(int dmg)
{
    hp -= dmg;
}