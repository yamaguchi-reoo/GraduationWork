#pragma once
#include "../CharacterBase.h"
#include "../../GameObject.h"

enum class PlayerState {
    Real,   // 実態
    Shadow  // 影状態
};

class Player :
    public CharacterBase
{
private:
	PlayerState state; // プレイヤーの状態（実態か影か）

	float shadow_gauge; //影化ゲージ
	float shadow_gauge_max; //影化ゲージの最大値
	float shadow_consumption; //影化ゲージの消費量


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

    //入力
    void HandleInput();

	// プレイヤーの状態の切り替え
    void SwitchState();

	// 影状態の更新
	void UpdateState();

	// UIの描画
	void DrawUI();

	// 影化ゲージの描画
    void DrawCircularShadowGauge(int center_x, int center_y, float scale);

	// 実態のHPゲージを描画
    void DrawRealHPGauge(int center_x, int center_y, float scale);

	// アークの描画
    void DrawArc(int center_x, int center_y, int inner_radius, int outer_radius, float start_angle, float end_angle, int color);
};

