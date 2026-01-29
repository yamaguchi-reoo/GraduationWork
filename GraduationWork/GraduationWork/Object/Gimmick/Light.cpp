#include "Light.h"
#include "DxLib.h"
#include "../../Object/ObjectManager.h"
#include "../Character/Player/Player.h"

#include <cmath>
#include <algorithm>


// Ray（光線）と AABB（矩形）の交差判定
static bool RayAABBIntersect(const Vector2D& ray_origin, const Vector2D& ray_dir,
	const Vector2D& box_min, const Vector2D& box_max, float& out_t)
{
	const float EPSILON = 1e-6f;
	float tmin = 0.0f;
	float tmax = FLT_MAX;

	// X軸方向の交差判定
	if (fabsf(ray_dir.x) > EPSILON)
	{
		float tx1 = (box_min.x - ray_origin.x) / ray_dir.x;
		float tx2 = (box_max.x - ray_origin.x) / ray_dir.x;
		if (tx1 > tx2) std::swap(tx1, tx2);
		tmin = Max(tmin, tx1);
		tmax = Min(tmax, tx2);
		if (tmin > tmax) return false;
	}
	else if (ray_origin.x < box_min.x || ray_origin.x > box_max.x)
	{
		return false;// 光が矩形のX範囲外なら交差しない
	}

	// Y軸方向
	if (fabsf(ray_dir.y) > EPSILON)
	{
		float ty1 = (box_min.y - ray_origin.y) / ray_dir.y;
		float ty2 = (box_max.y - ray_origin.y) / ray_dir.y;
		if (ty1 > ty2) std::swap(ty1, ty2);
		tmin = Max(tmin, ty1);
		tmax = Min(tmax, ty2);
		if (tmin > tmax) return false;
	}
	else if (ray_origin.y < box_min.y || ray_origin.y > box_max.y)
	{
		return false;
	}

	out_t = tmin; // 正方向の最初の交差点
	return true;
}


void Light::Initialize(Vector2D _location, Vector2D _box_size)
{
	object_type = LIGHT;
	__super::Initialize(_location, _box_size);

	width = box_size.x * 7.0f;  // 光の幅（影響範囲）
	height = box_size.y * 7.0f; // 光の高さ（影響範囲）

	light_data.origin = location;
	light_data.angle = 90.0f;			// 初期角度（上向き）
	light_data.min_angle = 60.0f;		// 最小角度
	light_data.max_angle = 120.0f;		// 最大角度
	light_data.rotate_speed = 20.0f;	// 回転速度（度/秒）
	light_data.direction = -1;			// 正方向回転
	light_data.moving = false;			// 動作中

	draw_priority = 15;
}

void Light::Update()
{
	__super::Update();

	// 光の自動回転処理
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
				// 光に当たった場合の処理
				this->OnHitCollision(obj);
			}
		}
	}
}

void Light::Draw(Vector2D offset, double rate)
{
	static const bool debug_draw_rays = true;
	if (!debug_draw_rays) return;

	const float tip_nudge = 1.4f;  // 光の先端を少し前に出すための微調整
	const float shrink = 1.0f;     // 将来的に光を縮小したい場合に使用可能

	// 光の起点を中心座標に変更
	Vector2D center = {
	(location.x + box_size.x * 0.5f) + 10.0f,
	(location.y + box_size.y * 0.5f) - 48.0f  // 少し上にオフセット
	};
	Vector2D world_tip = center;  // レイの起点も中心から
	Vector2D tip_screen = { world_tip.x - offset.x, world_tip.y - offset.y };

	float rad = light_data.angle * DX_PI_F / 180.0f; // ライトの角度をラジアンに変換
	float light_height = height;
	float light_width = width;

	const int ray_count = 16; // レイの本数
	const float half_angle = atanf(light_width / light_height) * 0.98f; // 光の広がり角の半分

	// 候補オブジェクト（ブロックと壁）をまとめる
	std::vector<GameObject*> candidate_objects;
	if (object_manager) {
		auto blocks = object_manager->GetObjects(BLOCK);
		auto walls = object_manager->GetObjects(WALL);
		candidate_objects.reserve(blocks.size() + walls.size());
		candidate_objects.insert(candidate_objects.end(), blocks.begin(), blocks.end());
		candidate_objects.insert(candidate_objects.end(), walls.begin(), walls.end());
	}

	// レイの終点を計算
	std::vector<Vector2D> end_points;
	end_points.reserve(ray_count + 1);

	for (int i = 0; i <= ray_count; ++i) {
		float t = (float)i / (float)ray_count;
		float angle = rad - half_angle + t * (2.0f * half_angle);
		Vector2D dir = { cosf(angle), sinf(angle) };

		// レイの最大距離
		float best_t = light_height;

		// 各オブジェクトとの交差判定
		for (auto obj : candidate_objects) {
			Vector2D pos = obj->GetLocation();
			Vector2D size = obj->GetBoxSize();
			Vector2D box_min = pos;
			Vector2D box_max = { pos.x + size.x, pos.y + size.y };

			float t_hit;
			if (RayAABBIntersect(world_tip, dir, box_min, box_max, t_hit)) {
				if (t_hit >= 0.0f && t_hit < best_t)
					best_t = t_hit; // 最も近い交点を採用
			}
		}

		// ワールド座標でのレイ終点
		end_points.push_back({ world_tip.x + dir.x * best_t, world_tip.y + dir.y * best_t });
	}

	// 光のtipを少し前に出す（ワールド座標）
	Vector2D central_dir = { cosf(rad), sinf(rad) };
	Vector2D p0_world = { world_tip.x + central_dir.x * tip_nudge, world_tip.y + central_dir.y * tip_nudge };
	Vector2D p0_screen = { p0_world.x - offset.x, p0_world.y - offset.y };

	// 描画開始
	SetDrawBlendMode(DX_BLENDMODE_ADD, 180);
	int color_light = GetColor(255, 255, 0);

	// レイごとに三角形を描画
	for (int i = 0; i < ray_count; ++i) {
		Vector2D p1 = end_points[i] - offset;
		Vector2D p2 = end_points[i + 1] - offset;

		// tip に寄せてサブピクセルの隙間を塞ぐ
		p1.x = p0_screen.x + (p1.x - p0_screen.x);
		p1.y = p0_screen.y + (p1.y - p0_screen.y);
		p2.x = p0_screen.x + (p2.x - p0_screen.x);
		p2.y = p0_screen.y + (p2.y - p0_screen.y);

		// 面積がほぼ 0 の場合は描画をスキップ
		float cross = (p1.x - p0_screen.x) * (p2.y - p0_screen.y) - (p1.y - p0_screen.y) * (p2.x - p0_screen.x);
		if (fabsf(cross) < 0.1f) continue;

		DrawTriangle(
			static_cast<int>(p0_screen.x), static_cast<int>(p0_screen.y),
			static_cast<int>(p1.x), static_cast<int>(p1.y),
			static_cast<int>(p2.x), static_cast<int>(p2.y),
			color_light,
			TRUE
		);
	}

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	// 当たり判定用に最後のレイ終点を保存
	last_ray_end_points = end_points;
}

void Light::Finalize()
{
	__super::Finalize();
}

void Light::OnHitCollision(GameObject* hit_object)
{
	Player* player = dynamic_cast<Player*>(hit_object);
	if (!player) return;

	if (!player->IsPlayerShadow()) return;

	// すでに死亡中なら何もしない
	if (player->IsDead()) return;

	player->SetPlayerActionDeath();
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
	if (last_ray_end_points.size() < 2) return false;

	Vector2D player_pos = player->GetLocation();
	Vector2D player_size = player->GetBoxSize();

	Vector2D corners[4] = {
		player_pos,
		{player_pos.x + player_size.x, player_pos.y},
		{player_pos.x, player_pos.y + player_size.y},
		{player_pos.x + player_size.x, player_pos.y + player_size.y}
	};

	Vector2D tip = location;

	for (size_t i = 0; i < last_ray_end_points.size() - 1; ++i)
	{
		Vector2D base_left = last_ray_end_points[i];
		Vector2D base_right = last_ray_end_points[i + 1];

		// Player の四隅が三角形内にあるか
		for (int c = 0; c < 4; ++c)
		{
			if (PointInTriangle(corners[c], tip, base_left, base_right))
				return true;
		}

		// 三角形の頂点が Player 矩形内にあるか
		Vector2D tri[3] = { tip, base_left, base_right };
		for (int t = 0; t < 3; ++t)
		{
			if (player_pos.x <= tri[t].x && tri[t].x <= player_pos.x + player_size.x &&
				player_pos.y <= tri[t].y && tri[t].y <= player_pos.y + player_size.y)
				return true;
		}
	}

	return false;
}


