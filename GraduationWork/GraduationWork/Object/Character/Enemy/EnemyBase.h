#pragma once
#include "../CharacterBase.h"
#include "../../GameObject.h"

// 敵の状態（今後拡張しやすいように enum 化）
enum class EnemyState
{
    Real,    // 実態
    Shadow   // 影状態
};

enum class EnemySituation
{
    Patrol, // 通常徘徊
    Alert,  // プレイヤーを見つけた直後
    Charge  // 突撃
};

class EnemyBase : public CharacterBase
{
protected:


    // ===== 基本ステータス =====
    int max_hp;          // 最大HP
    int damage;          // プレイヤーに与えるダメージ
    bool is_dead;        // 死亡フラグ
    bool is_alive;       // 生存フラグ

    // ===== 移動関連 =====
    float move_speed;    // 通常移動速度
    float charge_speed;  // 突撃時の速度
    bool moving_right;   // 向いている方向（true:右）

    // ===== AI状態 =====
    bool is_charging;    // 突撃中かどうか
    EnemyState state;    // 実態 / 影

    EnemySituation situation; //状態

    // ===== プレイヤー検知 =====
    Vector2D search_box_size; // 検知範囲（四角形）
    bool player_detected;     // プレイヤーを検知しているか


    float patrol_speed;

    float alert_time;
    float alert_limit;

    int charge_timer;
    int charge_limit;

protected:
    // プレイヤー検知処理
    void PlayerSearch();

    // 突撃開始処理
    void StartCharge(class Player* player);

    //徘徊
    void UpdatePatrol();

    //
    void StartAlert();
    void UpdateAlert();

    //
    void UpdateCharge();
    void EndCharge();

public:
    EnemyBase();
    virtual ~EnemyBase();

    // 基本処理
    virtual void Initialize(Vector2D _location, Vector2D _box_size) override;
    virtual void Update() override;
    virtual void Draw(Vector2D offset, double rate) override;
    virtual void OnHitCollision(GameObject* hit_object) override;

    // 状態切り替え
    virtual void SwitchState();

    // ダメージを受ける
    virtual void ReceiveDamage(int dmg);

    // 状態取得
    bool IsDead() const { return is_dead; }
    bool IsShadow() const { return state == EnemyState::Shadow; }
};
