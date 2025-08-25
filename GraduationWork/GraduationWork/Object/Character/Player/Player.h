#pragma once
#include "../CharacterBase.h"
#include "../../GameObject.h"

#include "Gauge.h"

enum class PlayerState {
    Real,   // 実態
    Shadow  // 影状態
};

struct AttackHitBox
{
    Vector2D position;
    Vector2D size;
    int frame; // フレーム数
};

class Player :
    public CharacterBase
{
private:

    //ジャンプ
    float jump_velocity;
    const float jump_strength; 

    //攻撃
    bool is_attacking;
    int attack_cooldown;
    const int attack_cooldown_max;

	int invincible_timer; // 無敵時間タイマー

	PlayerState state; // プレイヤーの状態（実態か影か）


	Gauge shadow_gauge; // 影化ゲージ
    Gauge hp_gauge; // 実態のHPゲージ

	//攻撃のヒットボックス
    std::vector<AttackHitBox> attack_hitboxes;
public:
    Player();
    ~Player();

    //初期化処理
    void Initialize(Vector2D _location, Vector2D _box_size) override;
    //更新処理
    void Update()override;
    //描画処理
    void Draw(Vector2D offset, double rate) override;
    //終了時処理
    void Finalize()override;

    //当たった時の挙動
    void OnHitCollision(GameObject* hit_object)override;

public:
    //入力
    void HandleInput();

    void UpdateJump();
    void UpdateAttack();

	// プレイヤーの状態の切り替え
    void SwitchState();

	// 影状態の更新
	void UpdateState();

	// UIの描画
	void DrawUI();

	// プレイヤーの状態を取得
    PlayerState GetPlayerState(){return state;}

    bool IsPlayerShadow() const override { return state == PlayerState::Shadow; }
};

