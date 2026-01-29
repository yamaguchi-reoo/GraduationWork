#include "ShadowHeal.h"
#include <DxLib.h>
#include "../Character/Player/Player.h"
#include "../Character/Player/Gauge.h"
#include "../ObjectManager.h"

void ShadowHeal::Initialize(Vector2D _location, Vector2D _box_size)
{
	object_type = SHADOWHEAL;

	__super::Initialize(_location, _box_size);

	image = LoadGraph("Resource/Images/Object/potion_471.png");
}

void ShadowHeal::Update()
{
	__super::Update();
}

void ShadowHeal::Draw(Vector2D offset, double rate)
{
	Vector2D screen_pos = location - offset;
	DrawRotaGraphF(
		screen_pos.x + (box_size.x / 2),
		screen_pos.y + (box_size.y / 2),
		2.0,    // 拡大率
		0.0,    // 回転角
		image,
		TRUE,   // 透過
		flip_flg
	);

#ifdef _DEBUG
	DrawBoxAA(screen_pos.x, screen_pos.y, screen_pos.x + box_size.x, screen_pos.y + box_size.y, GetColor(0, 255, 125), FALSE);
#endif
	//__super::Draw(screen_pos, rate);
}


void ShadowHeal::Finalize()
{
	__super::Finalize();
}



void ShadowHeal::OnHitCollision(GameObject* hit_object)
{
	const int GAUGE_RECOVER_AMOUNT = 200;

    int type = hit_object->GetObjectType();
	if (type == PLAYER) {
		Player* player = dynamic_cast<Player*>(hit_object);

		if (player) {

			//DrawFormatString(10, 200, GetColor(255, 0, 0), "Gauge HEAL SUCCESS! Value: %d", GAUGE_RECOVER_AMOUNT);

			// 1. プレイヤーからゲージオブジェクトの参照を取得 (Player::GetGauge()は必要)
			Gauge& player_gauge = player->GetGauge();

			// 2. 【新しい公開メソッド AddValue() で回復量を直接渡す】
			player_gauge.AddValue(GAUGE_RECOVER_AMOUNT);

			// アイテムを削除
			object_manager->RequestDeleteObject(this);
		}
	}
}
