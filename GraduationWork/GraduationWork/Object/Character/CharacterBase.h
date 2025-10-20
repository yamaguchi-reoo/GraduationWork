#pragma once
#include "../GameObject.h"
#include "../../Utility/UserTemplate.h"

#include <vector>

class CharacterBase :
	public GameObject
{
protected:
	int hp = 0;                // ヒットポイント（任意の値で調整可能）

	bool on_ground = false;           //地面にいるかどうか
	bool is_jumping = false;


	Vector2D velocity = { 0, 0 };     // 移動速度（x, y）
	const float gravity = 0.5f;       // 重力加速度（任意の値で調整可能）
	const float max_fall_speed = 10.0f; // 最大落下速度

	//std::vector<int> animation_data;//アニメーションデータ
public:
	//初期化処理
	virtual void Initialize(Vector2D _location, Vector2D _box_size) override;
	//更新処理
	virtual void Update()override;
	//描画処理
	virtual void Draw(Vector2D offset, double rate)  override;
	//終了時処理
	virtual void Finalize()override;

	virtual void OnHitCollision(GameObject* hit_object)override;

	//影のプレイヤーかどうか
	virtual bool IsPlayerShadow() const { return false; }

	bool IsOnGround() const { return on_ground; }

};

