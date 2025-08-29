#include "Plate.h"
#include "DxLib.h"


void Plate::Initialize(Vector2D _location, Vector2D _box_size)
{
	object_type = PLATE;


	__super::Initialize(_location, _box_size);

	TrapOn_flg = false;
}

void Plate::Update()
{
	__super::Update();
}

void Plate::Draw(Vector2D offset, double rate)
{
	Vector2D screen_pos = location - offset;
	//__super::Draw(screen_pos, rate);

	float plateHeight = 3.0f;  // 板の厚さ
	float blockHeight = box_size.y; // 1マスの高さ（CSVで使ってるブロックのサイズ）

	// 感圧板をマスの下に合わせる（浮かないようにする）
	float yOffset = blockHeight - plateHeight;

	//DrawBoxAA(
	//	screen_pos.x,
	//	screen_pos.y + yOffset,
	//	screen_pos.x + box_size.x,
	//	screen_pos.y + yOffset + plateHeight,
	//	GetColor(190, 140, 80),
	//	TRUE
	//);
#ifdef _DEBUG
	//DrawBoxAA(screen_pos.x, screen_pos.y, screen_pos.x + box_size.x, screen_pos.y + box_size.y, GetColor(255, 0, 255), TRUE);

	DrawBoxAA(screen_pos.x, screen_pos.y + 45, screen_pos.x + box_size.x, screen_pos.y + box_size.y + 45, GetColor(255, 0, 255), TRUE);

	// プレイヤーが上に乗っていたら文字を表示
	if (TrapOn_flg==true)
	{
		DrawString(100, 50, "Trap flg On", GetColor(255, 0, 0));
		TrapOn_flg = false;
	}
#endif // _DEBUG
}

void Plate::Finalize()
{
	__super::Finalize();
}

void Plate::OnHitCollision(GameObject* hit_object)
{
	int type = hit_object->GetObjectType();
	if (type != BLOCK && type != WALL && type != PUSHBLOCK && type != PLAYER) return;

	if (type == PLAYER) // プレイヤーと判定
	{
		// 当たり判定を描画と同じ位置に下げる
		float plateHeight = 3.0f;       // 板の厚さ
		float blockHeight = box_size.y; // 1マスの高さ
		float yOffset = blockHeight - plateHeight;

		Vector2D playerPos = hit_object->GetLocation();
		Vector2D playerSize = hit_object->GetBoxSize();

		// 判定矩形を下にずらしてチェック
		bool hit = !(playerPos.x + playerSize.x < location.x ||
			playerPos.x > location.x + box_size.x ||
			playerPos.y + playerSize.y < location.y + yOffset ||
			playerPos.y > location.y + yOffset + plateHeight);

		if (hit)
		{
			TrapOn_flg = true;
		}
	}
}
