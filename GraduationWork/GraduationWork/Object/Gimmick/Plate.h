#pragma once
#include "../GameObject.h"
#include "Light.h"

class Plate :
    public GameObject
{
public:
	bool TrapOn_flg = false;	
	Light* linked_light = nullptr;

	bool prev_TrapOn_flg = false;

	float press_offset;        // 現在の沈み量
	float press_offset_target; // 目標沈み量
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

	bool IsOnPlate(GameObject* obj) const;

};

