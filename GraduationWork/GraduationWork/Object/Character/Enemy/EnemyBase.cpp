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
    search_box_size(200.0f, 100.0f),
    player_detected(false)
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
    object_type = ENEMY;

    is_dead = false;
    is_alive = true;
    is_charging = false;
}

//====================================================
// 更新処理
//====================================================
void EnemyBase::Update()
{
    // プレイヤー検知
    PlayerSearch();

    // 突撃中か通常移動かで速度を切り替える
    if (is_charging)
    {
        velocity.x = moving_right ? charge_speed : -charge_speed;
    }
    else
    {
        velocity.x = moving_right ? move_speed : -move_speed;
    }

    // CharacterBase の物理処理（重力・衝突）
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
        TRUE
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
    if (type == BLOCK || type == WALL)
    {
        moving_right = !moving_right;

        // 突撃中なら解除
        if (is_charging)
        {
            is_charging = false;
            velocity.x = 0;
        }
    }
}

//====================================================
// プレイヤー検知処理（四角形）
//====================================================
void EnemyBase::PlayerSearch()
{
    if (!object_manager) return;

    Vector2D my_pos = GetLocation();
    Vector2D my_size = GetBoxSize();

    // 検知範囲の左上座標を計算
    Vector2D search_pos;
    search_pos.y = my_pos.y;

    if (moving_right)
        search_pos.x = my_pos.x + my_size.x;
    else
        search_pos.x = my_pos.x - search_box_size.x;

    // プレイヤー一覧を取得
    for (auto obj : object_manager->GetObjects(PLAYER))
    {
        Player* player = dynamic_cast<Player*>(obj);
        if (!player) continue;

        // 影状態のプレイヤーは無視
        if (player->IsPlayerShadow()) continue;

        Vector2D p_pos = player->GetLocation();
        Vector2D p_size = player->GetBoxSize();

        // AABB（四角形）判定
        bool hit =
            p_pos.x < search_pos.x + search_box_size.x &&
            p_pos.x + p_size.x > search_pos.x &&
            p_pos.y < search_pos.y + search_box_size.y &&
            p_pos.y + p_size.y > search_pos.y;

        if (hit)
        {
            player_detected = true;
            StartCharge(player);
            return;
        }
    }

    player_detected = false;
}

//====================================================
// 突撃開始
//====================================================
void EnemyBase::StartCharge(Player* player)
{
    if (!player) return;

    is_charging = true;

    // 向きは現在の向きのまま、前に突っ込むだけ
    velocity.x = moving_right ? charge_speed : -charge_speed;
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
