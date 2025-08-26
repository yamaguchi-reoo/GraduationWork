#pragma once
#include "../CharacterBase.h"
#include "../../GameObject.h"

enum class EnemyState {
    Real,   // é¿ë‘
    Shadow  // âeèÛë‘
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

public:
    EnemyBase();
    virtual ~EnemyBase();

    virtual void Initialize(Vector2D _location, Vector2D _box_size) override;
    virtual void Update() override;
    virtual void Draw(Vector2D offset, double rate) override;
    virtual void OnHitCollision(GameObject* hit_object) override;

    virtual void SwitchState();     // èÛë‘êÿÇËë÷Ç¶
    virtual void ReceiveDamage(int dmg);

    bool IsDead() const { return is_dead; }
    bool IsShadow() const { return state == EnemyState::Shadow; }
};