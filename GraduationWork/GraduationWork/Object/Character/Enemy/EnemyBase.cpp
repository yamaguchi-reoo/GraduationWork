#include <DxLib.h>
#include "EnemyBase.h"
#include "../../../Utility/UtilityList.h"
#include "../../../common.h"

#include "../../Object/ObjectManager.h"
#include "../Character/Player/Player.h"


EnemyBase::EnemyBase()
    : max_hp(3), damage(1), is_dead(false), is_alive(true),
    move_speed(2.0f), moving_right(true),
    state(EnemyState::Real),
    is_charging(false),
    charge_speed(5.0f),
    detect_range(150.0f)
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
}

void EnemyBase::Update()
{
    // ▼ 突撃中の移動
    if (is_charging)
    {
        ChargeMove();
        CharacterBase::Update();
        return;
    }

    // ▼ 通常巡回
    PatrolMove();

    CharacterBase::Update();

    // ▼ 死亡判定
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

    int color = (state == EnemyState::Real)
        ? GetColor(0, 255, 0)
        : GetColor(180, 80, 255);

    DrawBoxAA(
        screen_pos.x, screen_pos.y,
        screen_pos.x + box_size.x,
        screen_pos.y + box_size.y,
        color, TRUE
    );
    #ifdef _DEBUG // 検知範囲描画 
    DrawBoxAA( screen_pos.x - detect_range, screen_pos.y - 10, screen_pos.x + box_size.x + detect_range, screen_pos.y + box_size.y + 10, GetColor(255, 255, 0), FALSE ); 
    #endif
}

void EnemyBase::OnHitCollision(GameObject* hit_object)
{
    __super::OnHitCollision(hit_object);

    if (!hit_object) return;

    int type = hit_object->GetObjectType();

    // ▼ BLOCK / WALL で方向転換
    if (type == BLOCK || type == WALL)
    {
        moving_right = !moving_right;

        if (is_charging)
        {
            is_charging = false;
            velocity.x = 0;
        }
    }

    // ▼ プレイヤーを見つけたとき（取得なしで位置だけ記録）
    if (type == PLAYER && !is_charging)
    {
        Vector2D p = hit_object->GetLocation();

        // 横方向だけチェック（視界範囲）
        if (fabs(p.x - location.x) <= detect_range)
        {
            charge_target = p;
            StartCharge();
        }
    }
}

// =====================================================
// ▼ 通常巡回
// =====================================================
void EnemyBase::PatrolMove()
{
    velocity.x = moving_right ? move_speed : -move_speed;
}

// =====================================================
// ▼ 突撃開始
// =====================================================
void EnemyBase::StartCharge()
{
    is_charging = true;

    if (charge_target.x > location.x)
        velocity.x = charge_speed;
    else
        velocity.x = -charge_speed;
}

// =====================================================
// ▼ 突撃移動
// =====================================================
void EnemyBase::ChargeMove()
{
    if (fabs(location.x - charge_target.x) < 5.0f)
    {
        is_charging = false;
        velocity.x = 0;
    }
}

void EnemyBase::SwitchState()
{
    if (state == EnemyState::Real) state = EnemyState::Shadow;
    else state = EnemyState::Real;
}

void EnemyBase::ReceiveDamage(int dmg)
{
    hp -= dmg;
}
