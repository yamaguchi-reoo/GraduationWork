#include "Block.h"
#include <DxLib.h>

void Block::Initialize(Vector2D _location, Vector2D _box_size)
{
	__super::Initialize(_location, _box_size);

	object_type = BLOCK;

}

void Block::Update()
{
	__super::Update();
	// ブロックは特に更新処理はないが、必要ならここに追加
}

void Block::Draw(Vector2D offset, double rate)
{
	Vector2D screen_pos = location - offset;
	DrawBoxAA(screen_pos.x, screen_pos.y, screen_pos.x + box_size.x, screen_pos.y + box_size.y, GetColor(0, 0, 0), TRUE);

#ifdef _DEBUG
	//DrawFormatString(offset.x, offset.y, GetColor(255, 255, 255), "Block");
	//DrawBoxAA(screen_pos.x, screen_pos.y, screen_pos.x + box_size.x, screen_pos.y + box_size.y, GetColor(0, 0, 255), TRUE);
#endif // DEBUG
	__super::Draw(screen_pos, rate);
}

void Block::Finalize()
{
}
