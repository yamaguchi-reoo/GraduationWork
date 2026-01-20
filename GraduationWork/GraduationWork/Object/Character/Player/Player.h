#pragma once
#include "../CharacterBase.h"
#include "../../GameObject.h"
#include "Effect/Effect.h"
#include "PlayerSoundManager.h"

#include "Gauge.h"
#include <map>

// 攻撃判定
struct AttackHitBox 
{
    Vector2D position = { 0.0f };
    Vector2D size = { 0.0f };
    int frame = 0; 
};

class Player :
    public CharacterBase
{
private:

    // 内部状態
    enum class PlayerAction { Idle, Walk, Jump, Attack, Death };
    enum class PlayerState { Real, Shadow };

	PlayerAction action; // プレイヤーの現在の行動
    PlayerState state; // プレイヤーの状態（実態か影か）


    //ジャンプ
    float jump_velocity;
    const float jump_strength;

    //攻撃
    std::vector<AttackHitBox> attack_hitboxes;
    bool is_attacking;
    int attack_cooldown;
    const int attack_cooldown_max;

    // ゲージ・耐久
    Gauge shadow_gauge;     // 影化ゲージ
    Gauge hp_gauge;         // 実態のHPゲージ
    int invincible_timer;   // 無敵時間タイマー

    // エフェクト
	Effect effect; 
    

	//アニメーション
    std::map<PlayerAction, std::vector<int>> animation_shadow;
	std::map<PlayerAction, std::vector<int>> animation_real;

    std::map<PlayerAction, int> animation_frame_count;
    int animation_frame = 0; //現在のアニメーションフレーム

	// サウンドマネージャー
	SoundManager sound_manager;

public:
    Player();
    ~Player();

    Gauge& GetGauge();

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
	bool IsOverlapingWall(); 

public:
    //入力
    void HandleInput();
    void UpdateJump();
    void UpdateAttack();
	void UpdateAnimation();

    // プレイヤーの状態の切り替え
    void SwitchState();

    // 影状態の更新
    void UpdateState();

    // UIの描画
    void DrawUI();
    void LoadPlayerImage();
    void AddHP(int num);

    // プレイヤーの状態を取得
    PlayerState GetPlayerState() { return state; }
    void SetPlayerActionDeath() { action = PlayerAction::Death; }

    bool IsPlayerShadow() const override { return state == PlayerState::Shadow; }

};

