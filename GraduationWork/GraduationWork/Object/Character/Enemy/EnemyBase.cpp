#include <DxLib.h>
#include "EnemyBase.h"
#include "../../../Utility/UtilityList.h"
#include "../../../common.h"

#include "../../../Object/ObjectManager.h"
#include "../Player/Player.h"


//====================================================
// コンストラクタ
//====================================================
EnemyBase::EnemyBase()
    : max_hp(3),
    damage(1),
    is_dead(false),
    is_alive(true),
    move_speed(2.0f),
    charge_speed(6.0f),
    moving_right(true),
    is_charging(false),
    state(EnemyState::Real),
    situation(EnemySituation::Patrol),
    search_box_size(200.0f, 100.0f),
    player_detected(false),
    patrol_speed(1.0f),
    patrol_time(0.0f),
    patrol_limit(200.0f),
    alert_time (0.0f),
    alert_limit(30.0f),
    charge_timer(0),
    charge_limit(60)
{
}

//====================================================
EnemyBase::~EnemyBase()
{
}

//====================================================
// 初期化
//====================================================
void EnemyBase::Initialize(Vector2D _location, Vector2D _box_size)
{
    __super::Initialize(_location, _box_size);

    hp = max_hp;
    

    is_dead = false;
    is_alive = true;
    is_charging = false;
}

//====================================================
// 更新処理
//====================================================
void EnemyBase::Update()
{
    switch (situation)
    {
    case EnemySituation::Patrol:
        UpdatePatrol();
        PlayerSearch();   // 徘徊中だけ探索
        break;

    case EnemySituation::Alert:
        UpdateAlert();
        break;

    case EnemySituation::Charge:
        UpdateCharge();
        break;
    }

    __super::Update();

    // HPが0以下なら死亡
    if (hp <= 0)
    {
        is_dead = true;
        is_alive = false;
    }
}

//====================================================
// 描画処理
//====================================================
void EnemyBase::Draw(Vector2D offset, double rate)
{
    Vector2D screen_pos = location - offset;

    // 本体描画（色で状態を分ける）
    int color = (state == EnemyState::Real)
        ? GetColor(0, 255, 0)
        : GetColor(180, 80, 255);

    DrawBoxAA(
        screen_pos.x,
        screen_pos.y,
        screen_pos.x + box_size.x,
        screen_pos.y + box_size.y,
        color,
        FALSE
    );

#ifdef _DEBUG
    // ===== 検知範囲の可視化（デバッグ用） =====
    Vector2D search_pos;

    if (moving_right)
        search_pos = Vector2D(screen_pos.x + box_size.x, screen_pos.y);
    else
        search_pos = Vector2D(screen_pos.x - search_box_size.x, screen_pos.y);

    DrawBoxAA(
        search_pos.x,
        search_pos.y,
        search_pos.x + search_box_size.x,
        search_pos.y + search_box_size.y,
        GetColor(255, 255, 0),
        FALSE
    );

    DrawFormatString(
        screen_pos.x,
        screen_pos.y - 16,
        GetColor(255, 255, 255),
        "Charge:%d",
        is_charging
    );

#endif
}

//====================================================
// 衝突処理
//====================================================
void EnemyBase::OnHitCollision(GameObject* hit_object)
{
    __super::OnHitCollision(hit_object);

    int type = hit_object->GetObjectType();

    // 壁・ブロックに当たったら方向転換
    //if (type == BLOCK || type == WALL)
    //{
    //    moving_right = !moving_right;

    //    // 突撃中なら解除
    //    if (is_charging)
    //    {
    //        velocity.x = 0;
    //        EndCharge();
    //    }
    //}
}

//====================================================
// プレイヤー検知処理（四角形）
//====================================================
void EnemyBase::PlayerSearch()
{
    constexpr float DEG2RAD = 3.1415926535f / 180.0f;

    if (!object_manager) return;

    Vector2D enemy_pos = GetLocation();
    Vector2D enemy_size = GetBoxSize();

    for (auto obj : object_manager->GetObjects(PLAYER))
    {
        Player* player = dynamic_cast<Player*>(obj);
        if (!player) continue;

        // ===== プレイヤーが影状態なら無視 =====
        if (player->IsPlayerShadow()) continue;

        Vector2D player_pos = player->GetLocation();
        Vector2D player_size = player->GetBoxSize();

        //プレイヤーが「前」にいるか
        float dx = player_pos.x - enemy_pos.x;

        // 右向き → プレイヤーが左なら無視
        if (moving_right && dx < 0) continue;

        // 左向き → プレイヤーが右なら無視
        if (!moving_right && dx > 0) continue;


        //  視野角（FOV）チェック
        Vector2D forward = moving_right ? Vector2D(1, 0) : Vector2D(-1, 0);
        Vector2D toPlayer = player_pos - enemy_pos;

        forward.Normalize();
        toPlayer.Normalize();

        constexpr float VIEW_ANGLE = 60.0f;
        float limit = cosf(VIEW_ANGLE * 0.5f * DEG2RAD);

        float dot = forward.x * toPlayer.x + forward.y * toPlayer.y;
        if (dot < limit) continue; // 視野外

        // ===============================
        // ⑤ 検知範囲（四角形）
        // ===============================
        Vector2D search_pos;
        search_pos.y = enemy_pos.y;

        if (moving_right)
            search_pos.x = enemy_pos.x + enemy_size.x;
        else
            search_pos.x = enemy_pos.x - search_box_size.x;

        bool hit =
            player_pos.x < search_pos.x + search_box_size.x &&
            player_pos.x + player_size.x > search_pos.x &&
            player_pos.y < search_pos.y + search_box_size.y &&
            player_pos.y + player_size.y > search_pos.y;

        if (hit)
        {
            // ===============================
            // ⑥ 突撃開始
            // ===============================
            StartAlert();
            return;
        }
    }
}

//====================================================
// 突撃開始
//====================================================
void EnemyBase::StartCharge(Player* player)
{
    situation = EnemySituation::Charge;
    is_charging = true;
    charge_timer = 0;

    if (player->GetLocation().x > location.x)
        moving_right = true;
    else
        moving_right = false;

    velocity.x = moving_right ? charge_speed : -charge_speed;
}


void EnemyBase::UpdatePatrol()
{
    velocity.x = moving_right ? patrol_speed : -patrol_speed;
    patrol_time++;

    if (patrol_time >= patrol_limit)
    {
        patrol_time = 0;
        moving_right = !moving_right;
        
    }
}

void EnemyBase::StartAlert()
{
    situation = EnemySituation::Alert;
    alert_time = 0.0f;
    velocity.x = 0;
}

void EnemyBase::UpdateAlert()
{
    alert_time++;

    if (alert_time >= alert_limit)
    {
        situation = EnemySituation::Charge;
        alert_time == 0;
    }
}

void EnemyBase::UpdateCharge()
{
    charge_timer++;

    velocity.x = moving_right ? charge_speed : -charge_speed;

    if (charge_timer > charge_limit)
    {
        EndCharge();
    }
}

void EnemyBase::EndCharge()
{
    is_charging = false;
    velocity.x = 0;
    charge_timer = 0;
    situation = EnemySituation::Patrol;
}

//====================================================
// 状態切り替え
//====================================================
void EnemyBase::SwitchState()
{
    if (state == EnemyState::Real)
        state = EnemyState::Shadow;
    else
        state = EnemyState::Real;
}

//====================================================
// ダメージ処理
//====================================================
void EnemyBase::ReceiveDamage(int dmg)
{
    hp -= dmg;
}
