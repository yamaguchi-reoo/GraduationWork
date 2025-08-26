#pragma once
#include "../CharacterBase.h"
#include "../../GameObject.h"

class Enemy :
    public CharacterBase
{
private:
    int max_hp;
    int damage;
    bool is_dead;
    bool is_alive;

public:
    Enemy();
    ~Enemy();

    //初期化処理
    void Initialize(Vector2D _location, Vector2D _box_size) override;
    //更新処理
    void Update()override;
    //描画処理
    void Draw(Vector2D offset, double rate) override;
    //終了時処理
    void Finalize()override;


    // 衝突処理
    void OnHitCollision(GameObject* hit_object) override;

    // ダメージ処理
    void ReceiveDamage(int dmg);

    // 死亡チェック
    bool IsDead() const { return is_dead; }

};