#pragma once
#include "../GameObject.h"
class Light :
    public GameObject
{
private:
	
	Vector2D pivot;
	float angle;
	float length;
	float Draw_length;
	int beam_width;
	
	float target_angle;  // 目標角度
	float angle_speed;   // 角度の変化速度

	Vector2D corner[4];
	
	// 当たり判定用（地面付近に横長で置きたい）
// 例えば pivot より少し下（y方向へ）に横幅の広い矩形を設定
	Vector2D hit_box_pos;   // 当たり判定矩形の左上座標
	Vector2D hit_box_size;  // 当たり判定矩形の幅・高さ

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

	void DrawLightColumn(Vector2D pivot, float width, float length, float angle_rad);

	void DrawParallelogramFilled(Vector2D p0, Vector2D p1, Vector2D p2, Vector2D p3, unsigned int color);
};

