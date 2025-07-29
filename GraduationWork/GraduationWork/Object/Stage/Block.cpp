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
	__super::Draw(offset, rate);

#ifdef _DEBUG
	//DrawFormatString(offset.x, offset.y, GetColor(255, 255, 255), "Block");
#endif // DEBUG

}

void Block::Finalize()
{
}
