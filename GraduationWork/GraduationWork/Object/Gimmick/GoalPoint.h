#pragma once
#include "../GameObject.h"

class GoalPoint :
	public GameObject
{ 
public:
	bool reached = false; // ゴールに到達したかどうかのフラグ

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
};