#include "Light.h"
#include "DxLib.h"
#include "../../Object/ObjectManager.h"
#include "../Character/Player/Player.h"

#include <cmath>

void Light::Initialize(Vector2D _location, Vector2D _box_size)
{
	object_type = LIGHT;
	__super::Initialize(_location, _box_size);

	width = box_size.x * 5.0f;  // 光の幅（影響範囲）
	height = box_size.y * 5.0f; // 光の高さ（影響範囲）

	light_data.origin = location;
	light_data.angle = 90.0f;			// 初期角度（上向き）
	light_data.min_angle = 60.0f;		// 最小角度
	light_data.max_angle = 120.0f;		// 最大角度
	light_data.rotate_speed = 30.0f;	// 回転速度（度/秒）
	light_data.direction = 1;			// 正方向回転
	light_data.moving = false;			// 動作中
}

void Light::Update()
{
	__super::Update();

	if (light_data.moving)
	{
		light_data.angle += light_data.rotate_speed * (1.0f / 60.0f) * light_data.direction; 

		if(light_data.angle >= light_data.max_angle)
		{
			light_data.angle = light_data.max_angle;
			light_data.direction = -1; // 逆方向に回転
		}
		else if (light_data.angle <= light_data.min_angle)
		{
			light_data.angle = light_data.min_angle;
			light_data.direction = 1; // 正方向に回転
		}
	}

	// ObjectManager 経由で Player を取得
	if (object_manager)
	{
		for (auto obj : object_manager->GetObjects(PLAYER))
		{
			if (CheckLightCollision(obj))
			{
				this->OnHitCollision(obj); // 光に対する反応（削除など）
			}
		}
	}

	// 床との当たり判定で高さを制限
	max_height = height; // 一旦元の高さにリセット
	if (object_manager)
	{
		for (auto obj : object_manager->GetObjects(BLOCK)) // 床オブジェクト
		{
			Vector2D obj_pos = obj->GetLocation();
			Vector2D obj_size = obj->GetBoxSize();

			// tip の座標計算
			float rad = light_data.angle * DX_PI_F / 180.0f;
			float base_half = width;
			Vector2D tip = location;

			float tip_x = tip.x + cos(rad) * height; // 仮の先端X
			float tip_y = tip.y + sin(rad) * height; // 仮の先端Y

			// 光の先端が床に届く場合
			if (tip_x >= obj_pos.x && tip_x <= obj_pos.x + obj_size.x &&
				tip_y >= obj_pos.y)
			{
				float distance_to_floor = obj_pos.y - tip.y;
				if (distance_to_floor < max_height)
					max_height = distance_to_floor; // 床までの距離に制限
			}
		}
	}
}

void Light::Draw(Vector2D offset, double rate)
{
	Vector2D tip = location - offset;
	float rad = light_data.angle * DX_PI_F / 180.0f;
	float half_width = width;

	const int SEGMENTS = 20;
	std::vector<Vector2D> base_points;

	// 光の先端
	float effective_height = height; // 最大高さ
	Vector2D top = {
		tip.x + cos(rad) * effective_height,
		tip.y + sin(rad) * effective_height
	};

	// 左右の基底点（tip より少し下に設定してにじみを防ぐ）
	Vector2D left_base = { top.x - half_width, top.y + 0.5f };
	Vector2D right_base = { top.x + half_width, top.y + 0.5f };

	// SEGMENTS で分割
	for (int i = 0; i <= SEGMENTS; i++)
	{
		float t = (float)i / SEGMENTS;
		Vector2D pt;
		pt.x = left_base.x + (right_base.x - left_base.x) * t;
		pt.y = left_base.y;

		// 床に沿わせる
		if (object_manager)
		{
			for (auto block : object_manager->GetObjects(BLOCK))
			{
				Vector2D bpos = block->GetLocation();
				Vector2D bsize = block->GetBoxSize();

				if (pt.x >= bpos.x && pt.x <= bpos.x + bsize.x)
				{
					if (pt.y < bpos.y && bpos.y > tip.y)
						pt.y = bpos.y;
				}
			}
		}

		base_points.push_back(pt);
	}

	SetDrawBlendMode(DX_BLENDMODE_ADD, 128);

	// tip と分割点で三角形描画（int にキャスト）
	for (int i = 0; i < SEGMENTS; i++)
	{
		DrawTriangle(
			static_cast<int>(tip.x), static_cast<int>(tip.y),
			static_cast<int>(base_points[i].x), static_cast<int>(base_points[i].y),
			static_cast<int>(base_points[i + 1].x), static_cast<int>(base_points[i + 1].y),
			GetColor(255, 255, 0),
			TRUE
		);
	}

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void Light::Finalize()
{
	__super::Finalize();
}

void Light::OnHitCollision(GameObject* hit_object)
{
	 // hit_object が Player かチェック
	Player* player = dynamic_cast<Player*>(hit_object);
	if (player && player->IsPlayerShadow())
	{
		// 影状態のプレイヤーに当たったら Light を削除
		if (object_manager)
		{
			object_manager->RequestDeleteObject(player);
		}
	}

}

bool Light::PointInTriangle(const Vector2D& p, const Vector2D& a, const Vector2D& b, const Vector2D& c)
{
	float denom = (b.y - c.y) * (a.x - c.x) + (c.x - b.x) * (a.y - c.y);
	float alpha = ((b.y - c.y) * (p.x - c.x) + (c.x - b.x) * (p.y - c.y)) / denom;
	float beta = ((c.y - a.y) * (p.x - c.x) + (a.x - c.x) * (p.y - c.y)) / denom;
	float gamma = 1.0f - alpha - beta;

	return alpha >= 0 && beta >= 0 && gamma >= 0;
}


bool Light::CheckLightCollision(GameObject* player)
{
	Vector2D player_pos = player->GetLocation();
	Vector2D player_size = player->GetBoxSize();

	// Player の4隅
	Vector2D corners[4] = {
		player_pos,
		{player_pos.x + player_size.x, player_pos.y},
		{player_pos.x, player_pos.y + player_size.y},
		{player_pos.x + player_size.x, player_pos.y + player_size.y}
	};

	// 光の二等辺三角形頂点
	float rad = light_data.angle * DX_PI_F / 180.0f;
	float h = height;          // 高さ
	float b = width;           // 底辺半幅

	Vector2D tip = location;
	Vector2D base_left = {
		tip.x + cos(rad) * h - sin(rad) * b,
		tip.y + sin(rad) * h + cos(rad) * b
	};
	Vector2D base_right = {
		tip.x + cos(rad) * h + sin(rad) * b,
		tip.y + sin(rad) * h - cos(rad) * b
	};

	// Player の四隅のどれかが三角形内にあるかチェック
	for (int i = 0; i < 4; ++i)
	{
		if (PointInTriangle(corners[i], tip, base_left, base_right))
		{
			return true;
		}
	}

	// 三角形の頂点が Player 矩形内にある場合も衝突
	Vector2D tri_vertices[3] = { tip, base_left, base_right };
	for (int i = 0; i < 3; ++i)
	{
		if (player_pos.x <= tri_vertices[i].x && tri_vertices[i].x <= player_pos.x + player_size.x &&
			player_pos.y <= tri_vertices[i].y && tri_vertices[i].y <= player_pos.y + player_size.y)
		{
			return true;
		}
	}

	return false;
}
