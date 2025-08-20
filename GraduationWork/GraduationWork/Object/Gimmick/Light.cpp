#include "Light.h"
#include "DxLib.h"
#include <cmath>

void Light::Initialize(Vector2D _location, Vector2D _box_size)
{
	object_type = LIGHT;
	__super::Initialize(_location, _box_size);

	//pivot = { _location.x + _box_size.x / -70.0f, _location.y-250.0f }; // 柱の上端中央
	pivot = _location;

	length = _box_size.y;
	Draw_length = 150.0f;
	angle = 0.5f;

	// 光の柱の太さ（横幅）
	beam_width = 28.0f;

	// AABBとしての当たり判定サイズ
	hit_box = Vector2D(beam_width, length);
}

void Light::Update()
{
	__super::Update();

	// 将来の利用に向けて方向ベクトル（未使用）
	Vector2D direction = { sin(angle), cos(angle) };

	// 当たり判定だけ更新（描画とは別管理）
	float hit_box_width = 150.0f;
	float hit_box_height = 30.0f;

	hit_box_pos.x = pivot.x - hit_box_width / 2;
	hit_box_pos.y = pivot.y + length - hit_box_height / 2;

	hit_box_size.x = hit_box_width;
	hit_box_size.y = hit_box_height;
}

void Light::Draw(Vector2D offset, double rate)
{
	Vector2D screen_pos = location - offset;
	__super::Draw(screen_pos, rate);

	// pivotのスクリーン座標
	float px = pivot.x - offset.x;
	float py = pivot.y - offset.y;

	// ここで四角形の幅・長さ・角度を指定して描く
	DrawLightColumn({ px, py }, beam_width, Draw_length, angle);

#ifdef _DEBUG
	DrawBoxAA(390, 290, 410, 310, GetColor(255, 0, 0), FALSE); // 目印の四角
#endif
}

void Light::Finalize()
{
	__super::Finalize();
}

void Light::OnHitCollision(GameObject* hit_object)
{
}

void Light::DrawLightColumn(Vector2D pivot, float width, float length, float angle_rad)
{
	float halfWidth = width * 0.5f;

	Vector2D widthDir = { cosf(angle_rad), sinf(angle_rad) };
	Vector2D lengthDir = { -sinf(angle_rad), cosf(angle_rad) };  // 90度回転したベクトル

	Vector2D topLeft = pivot - widthDir * halfWidth;
	Vector2D topRight = pivot + widthDir * halfWidth;
	Vector2D bottomLeft = topLeft + lengthDir * length;
	Vector2D bottomRight = topRight + lengthDir * length;


	DrawParallelogramFilled(topLeft, topRight, bottomRight, bottomLeft, GetColor(255, 255, 100));
}

void Light::DrawParallelogramFilled(Vector2D p0, Vector2D p1, Vector2D p2, Vector2D p3, unsigned int color)
{
	// 三角形１：p0, p1, p2
	DrawTriangle(
		static_cast<int>(p0.x), static_cast<int>(p0.y),
		static_cast<int>(p1.x), static_cast<int>(p1.y),
		static_cast<int>(p2.x), static_cast<int>(p2.y),
		color, TRUE
	);

	// 三角形２：p0, p2, p3
	DrawTriangle(
		static_cast<int>(p0.x), static_cast<int>(p0.y),
		static_cast<int>(p2.x), static_cast<int>(p2.y),
		static_cast<int>(p3.x), static_cast<int>(p3.y),
		color, TRUE
	);
}



