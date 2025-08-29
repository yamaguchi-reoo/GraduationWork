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
}

void Light::Update()
{
	__super::Update();

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
}

void Light::Draw(Vector2D offset, double rate)
{
	Vector2D screen_pos = location - offset;

	// デバッグ用に三角形を描画
#ifdef _DEBUG
	Vector2D tip = screen_pos;
	Vector2D base_left = { screen_pos.x - box_size.x / 2.0f, screen_pos.y + box_size.y };
	Vector2D base_right = { screen_pos.x + box_size.x / 2.0f, screen_pos.y + box_size.y };

	DrawLineAA(tip.x, tip.y, base_left.x, base_left.y, GetColor(255, 255, 0), 2.0f);
	DrawLineAA(tip.x, tip.y, base_right.x, base_right.y, GetColor(255, 255, 0), 2.0f);
	DrawLineAA(base_left.x, base_left.y, base_right.x, base_right.y, GetColor(255, 255, 0), 2.0f);

	// 透過で塗りつぶし（軽く黄色）
	DrawTriangleAA(tip.x, tip.y, base_left.x, base_left.y, base_right.x, base_right.y, GetColor(255, 255, 0), TRUE);
#endif

	//__super::Draw(screen_pos, rate);
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

	// Light の三角形頂点
	Vector2D tip = location; // 光の先端
	Vector2D base_left = { location.x - box_size.x / 2.0f, location.y + box_size.y };
	Vector2D base_right = { location.x + box_size.x / 2.0f, location.y + box_size.y };

	for (int i = 0; i < 4; ++i)
	{
		if (PointInTriangle(corners[i], tip, base_left, base_right))
		{
			return true; 
		}
	}

	return false;
}



