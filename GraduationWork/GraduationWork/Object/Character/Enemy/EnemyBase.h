#pragma once
#include "../CharacterBase.h"
#include "../../GameObject.h"

enum class EnemyState {
    Real,   // 実態
    Shadow  // 影状態
};

class EnemyBase : public CharacterBase
{
protected:
    int max_hp;
    int damage;
    bool is_dead;
    bool is_alive;
    float move_speed;
    bool moving_right;
    EnemyState state;

    // 検知 & 突撃用
    bool is_charging;          // 突撃中？
    float charge_speed;        // 突撃速度
    float detect_range;        // プレイヤー検知距離（横方向）
    Vector2D charge_target;    // 記録した突撃目的地

public:
    EnemyBase();
    virtual ~EnemyBase();

    virtual void Initialize(Vector2D _location, Vector2D _box_size) override;
    virtual void Update() override;
    virtual void Draw(Vector2D offset, double rate) override;
    virtual void OnHitCollision(GameObject* hit_object) override;

    virtual void SwitchState();
    virtual void ReceiveDamage(int dmg);

    bool IsDead() const { return is_dead; }
    bool IsShadow() const { return state == EnemyState::Shadow; }

    void PatrolMove();
    void StartCharge();
    void ChargeMove();
};
