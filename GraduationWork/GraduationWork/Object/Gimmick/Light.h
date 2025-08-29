#pragma once
#include "../GameObject.h"

struct LightData {
	Vector2D origin = { 0.0f,0.0f };      // 頂点（固定）
	float angle = 0.0f;          // 現在の向き
	float min_angle = 0.0f;      // 最小角度
	float max_angle = 0.0f;      // 最大角度
	float rotate_speed = 0.0f;   // 回転速度（度/秒）
	bool moving = false;         // 感圧版で動作中かどうか
	int direction = 1;			 // 1: 正方向回転, -1: 逆方向回転
};

class Light :
    public GameObject
{
private:
	Vector2D apex = { 0.0f };
	float width = 0.0f;
	float height = 0.0f;
	float max_height = 0.0f;

	LightData light_data;

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

private:
	bool PointInTriangle(const Vector2D& p, const Vector2D& a, const Vector2D& b, const Vector2D& c);

	bool CheckLightCollision(GameObject* obj);
	
};

