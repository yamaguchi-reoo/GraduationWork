#include <DxLib.h>
#include "Player.h"
#include "../../../Utility/UtilityList.h"


Player::Player()
{
}

Player::~Player()
{
}

void Player::Initialize(Vector2D _location, Vector2D _box_size)
{
	__super::Initialize(_location, _box_size);

	object_type = PLAYER;
}

void Player::Update()
{
	__super::Update();
}

void Player::Draw(Vector2D offset, double rate)
{
	__super::Draw(offset, rate);


#ifdef _DEBUG
	DrawFormatString(offset.x, offset.y, GetColor(255, 255, 255), "Player");
#endif // DEBUG

	//DrawBoxAA(offset.x, offset.y, offset.x + box_size.x, offset.y +box_size.y, GetColor(255, 0, 0), TRUE);
}

void Player::Finalize()
{
	__super::Finalize();
}

void Player::OnHitCollision(GameObject* hit_object)
{
	__super::OnHitCollision(hit_object);
}
