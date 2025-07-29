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
	HandleInput();
	__super::Update();
}

void Player::Draw(Vector2D offset, double rate)
{
	__super::Draw(offset, rate);


#ifdef _DEBUG
	DrawFormatString(offset.x, offset.y, GetColor(255, 255, 255), "Player");
	//DrawFormatString(location.x, location.y, GetColor(255, 255, 255), "Player");
#endif // DEBUG

	DrawBoxAA(offset.x, offset.y, offset.x + box_size.x, offset.y +box_size.y, GetColor(255, 0, 0), TRUE);
}

void Player::Finalize()
{
	__super::Finalize();
}

void Player::OnHitCollision(GameObject* hit_object)
{
	__super::OnHitCollision(hit_object);
}

void Player::HandleInput()
{
	InputManager* input = InputManager::GetInstance();

	if (input->GetButton(XINPUT_BUTTON_DPAD_LEFT))
	{
		// ���Ɉړ�
		velocity.x = -5.0f; // �C�ӂ̑��x�Œ���
		flip_flg = TRUE; // �������ɐݒ�
	}
	else if (input->GetButton(XINPUT_BUTTON_DPAD_RIGHT))
	{
		// �E�Ɉړ�
		velocity.x = 5.0f; // �C�ӂ̑��x�Œ���
		flip_flg = FALSE; // �E�����ɐݒ�
	}
	else
	{
		// ��~
		velocity.x = 0.0f;
	}
}
