#pragma once
#include "../GameObject.h"
#include "../../Utility/Vector2D.h"

class PushBlock :
    public GameObject
{
private:

	bool is_moving = false; // 移動中かどうか
	Vector2D move_direction = { 0.0f,0.0f };
	Vector2D move_speed = 1.5f; // 移動速度
	float move_progress = 0.0f; // 移動の進捗（0.0f から 1.0f）
	float move_distance = 12.0f; // 移動距離

	Vector2D current_move_dir = { 0, 0 };

	const float gravity = 0.5f;       // 重力加速度
	const float terminal_velocity = 8.0f; // 最大落下速度
	bool on_ground = false;         // 地面に接しているか
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

public:
	void StartMove(Vector2D dir); // 移動開始処理
	void MoveUpdate();            // 移動処理

};

