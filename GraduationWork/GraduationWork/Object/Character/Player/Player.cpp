#include <DxLib.h>
#include "Player.h"
#include "../../../Utility/UtilityList.h"
#include "../../../common.h"

namespace {
	constexpr int BASE_RADIUS = 60;
	constexpr int BASE_THICKNESS = 30;
}

Player::Player() : 
	state(PlayerState::Real), 
	jump_velocity(0.0f),
	jump_strength(15.0f),
	is_attacking(false),
	attack_cooldown(0),
	attack_cooldown_max(20),
	shadow_gauge(),
	hp_gauge()
{
}


Player::~Player()
{
}

void Player::Initialize(Vector2D _location, Vector2D _box_size)
{
	__super::Initialize(_location, _box_size);

	object_type = PLAYER;

	hp = 2;

	is_jumping = false;

	shadow_gauge.Initialize(GaugeType::CircularFill, 1200, 1200, 0, GetColor(180, 80, 255));
	hp_gauge.Initialize(GaugeType::CircularSection, 3, hp, 3, GetColor(255, 0, 0));
}

void Player::Update()
{
	HandleInput();
	UpdateState(); // 影状態の更新
	UpdateJump(); // ジャンプの更新

	hp_gauge.SetValue(hp);
	shadow_gauge.SetValue(1200); 

	__super::Update();
}

void Player::Draw(Vector2D offset, double rate)
{
	__super::Draw(offset, rate);

	if (state == PlayerState::Real)
	{
		// 実態の描画
		DrawBoxAA(offset.x, offset.y, offset.x + box_size.x, offset.y + box_size.y, GetColor(255, 0, 0), TRUE);
	}
	else if (state == PlayerState::Shadow)
	{
		// 影状態の描画
		DrawBoxAA(offset.x, offset.y, offset.x + box_size.x, offset.y + box_size.y, GetColor(180, 80, 255), TRUE);
	}

	DrawUI();

#ifdef _DEBUG
	DrawFormatString(offset.x, offset.y, GetColor(255, 255, 255), "Player");
	DrawFormatString(0, 40, GetColor(255, 255, 255), "State: %s", (state == PlayerState::Real) ? "Real" : "Shadow");
	//DrawFormatString(0, 60, GetColor(255, 255, 255), "Gauge: %f", shadow_gauge);
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

	if (input->GetButtonDown(XINPUT_BUTTON_A) && !is_jumping && state == PlayerState::Real)
	{
		// ジャンプ
		is_jumping = true;
		on_ground = false;
		jump_velocity = -jump_strength; // ジャンプの初速を設定
	}


}

void Player::UpdateJump()
{
	if (is_jumping)
	{
		velocity.y = jump_velocity;
		jump_velocity += gravity;
	}
}

void Player::UpdateAttack()
{
}

void Player::SwitchState()
{
	if (state == PlayerState::Real)
	{
		state = PlayerState::Shadow;

	}
	else if (state == PlayerState::Shadow)
	{
		// 実態に戻る
		state = PlayerState::Real;
	}
}

void Player::UpdateState()
{
	if (state == PlayerState::Shadow)
	{
		// ゲージが尽きたので実態に戻す
		//SwitchState();
	}
	else if (state == PlayerState::Real)
	{
	}
}

void Player::DrawUI()
{
	constexpr int MARGIN = 20;
	constexpr int RADIUS = 60;

	int center_x = SCREEN_WIDTH - RADIUS - MARGIN;
	int center_y_top = RADIUS + MARGIN;
	int center_y_bottom = center_y_top + 80;

	constexpr float LARGE = 1.0f;
	constexpr float SMALL = 0.5f;

	if (state == PlayerState::Real)
	{
		// HPゲージ（セクション分割）
		hp_gauge.Draw(center_x, center_y_top, LARGE);

		// 影ゲージ（連続塗りつぶし）
		shadow_gauge.Draw(center_x, center_y_bottom, SMALL);
	}
	else
	{
		// 影状態のとき影ゲージを大きく表示
		shadow_gauge.Draw(center_x, center_y_top, LARGE);

		// HPゲージを小さく表示
		hp_gauge.Draw(center_x, center_y_bottom, SMALL);
	}
}
