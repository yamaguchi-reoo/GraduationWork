#include <DxLib.h>
#include "Player.h"
#include "../../../Utility/UtilityList.h"


Player::Player() : state(PlayerState::Real), shadow_gauge(1200.0f), shadow_gauge_max(1200.0f), shadow_consumption(10.0f)
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
	UpdateState(); // 影状態の更新

	__super::Update();
}

void Player::Draw(Vector2D offset, double rate)
{
	__super::Draw(offset, rate);


#ifdef _DEBUG
	DrawFormatString(offset.x, offset.y, GetColor(255, 255, 255), "Player");
	DrawFormatString(0, 40, GetColor(255, 255, 255), "State: %s", (state == PlayerState::Real) ? "Real" : "Shadow");
	DrawFormatString(0, 60, GetColor(255, 255, 255), "Gauge: %f", shadow_gauge);
#endif // DEBUG
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
		// 左に移動
		velocity.x = -5.0f; // 任意の速度で調整
		flip_flg = TRUE; // 左向きに設定
	}
	else if (input->GetButton(XINPUT_BUTTON_DPAD_RIGHT))
	{
		// 右に移動
		velocity.x = 5.0f; // 任意の速度で調整
		flip_flg = FALSE; // 右向きに設定
	}
	else if (input->GetButtonDown(XINPUT_BUTTON_RIGHT_SHOULDER))
	{
		// Aボタンで状態を切り替え
		SwitchState();
	}
	else
	{
		// 停止
		velocity.x = 0.0f;
	}
}

void Player::SwitchState()
{
	if (state == PlayerState::Real)
	{
		state = PlayerState::Shadow;
	}
	else
	{
		state = PlayerState::Real;
	}
}

void Player::UpdateState()
{
	if (state == PlayerState::Shadow)
	{
		shadow_gauge -= shadow_consumption; // 影化ゲージを消費
		if (shadow_gauge < 0.0f)
		{
			shadow_gauge = 0.0f; // ゲージが0未満にならないようにする

			state = PlayerState::Real; // ゲージが0になったら実態に戻る
		}
	}
	else
	{
		shadow_gauge += shadow_consumption * 0.1f; // ゲージを徐々に回復
		if (shadow_gauge > shadow_gauge_max)
		{
			shadow_gauge = shadow_gauge_max; // 最大値を超えないようにする
		}
	}

}
