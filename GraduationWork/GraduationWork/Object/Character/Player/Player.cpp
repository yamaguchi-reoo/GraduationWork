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
	UpdateState(); 
	UpdateJump(); 

	hp_gauge.SetValue(hp); 

	__super::Update();
}

void Player::Draw(Vector2D offset, double rate)
{
	__super::Draw(offset, rate);

	if (state == PlayerState::Real)
	{
		// ŽÀ‘Ô‚Ì•`‰æ
		DrawBoxAA(offset.x, offset.y, offset.x + box_size.x, offset.y + box_size.y, GetColor(255, 0, 0), TRUE);
	}
	else if (state == PlayerState::Shadow)
	{
		// ‰eó‘Ô‚Ì•`‰æ
		DrawBoxAA(offset.x, offset.y, offset.x + box_size.x, offset.y + box_size.y, GetColor(180, 80, 255), TRUE);
	}

	DrawUI();

#ifdef _DEBUG
	DrawFormatStringF(offset.x, offset.y, GetColor(255, 255, 255), "Player");
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
		// ¶‚ÉˆÚ“®
		velocity.x = -5.0f; // ”CˆÓ‚Ì‘¬“x‚Å’²®
		flip_flg = TRUE; // ¶Œü‚«‚ÉÝ’è
	}
	else if (input->GetButton(XINPUT_BUTTON_DPAD_RIGHT))
	{
		// ‰E‚ÉˆÚ“®
		velocity.x = 5.0f; // ”CˆÓ‚Ì‘¬“x‚Å’²®
		flip_flg = FALSE; // ‰EŒü‚«‚ÉÝ’è
	}
	else
	{
		// ’âŽ~
		velocity.x = 0.0f;
	}

	
	//‰eó‘Ô
	if (input->GetButtonDown(XINPUT_BUTTON_RIGHT_SHOULDER))
	{
		// Aƒ{ƒ^ƒ“‚Åó‘Ô‚ðØ‚è‘Ö‚¦
		SwitchState();
	}

	if (input->GetButtonDown(XINPUT_BUTTON_A) && !is_jumping && state == PlayerState::Real)
	{
		// ƒWƒƒƒ“ƒv
		is_jumping = true;
		on_ground = false;
		jump_velocity = -jump_strength; // ƒWƒƒƒ“ƒv‚Ì‰‘¬‚ðÝ’è
	}


	if(input->GetKeyDown(KEY_INPUT_Z) && state == PlayerState::Real)
	{
		hp--;
		if (hp <= 0)
		{
			hp = 0;
		}
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
		state = PlayerState::Real;
	}
}

void Player::UpdateState()
{
	// ‰eó‘Ô‚Ì‚Æ‚«‚Í‰eƒQ[ƒW‚ðXV
	if (state == PlayerState::Shadow)
	{
		// ‰eƒQ[ƒW‚ðÁ”ï
		shadow_gauge.Update(true);
		if (shadow_gauge.IsEmpty()) 
		{
			SwitchState();
		}
	}
	else if (state == PlayerState::Real)
	{
		// ŽÀ‘Ô‚Ì‚Æ‚«‚Í‰eƒQ[ƒW‚ð‰ñ•œ
		shadow_gauge.Update(false);
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
		// HPƒQ[ƒWiƒZƒNƒVƒ‡ƒ“•ªŠ„j
		hp_gauge.Draw(center_x, center_y_top, LARGE);

		// ‰eƒQ[ƒWi˜A‘±“h‚è‚Â‚Ô‚µj
		shadow_gauge.Draw(center_x, center_y_bottom, SMALL);
	}
	else
	{
		// ‰eó‘Ô‚Ì‚Æ‚«‰eƒQ[ƒW‚ð‘å‚«‚­•\Ž¦
		shadow_gauge.Draw(center_x, center_y_top, LARGE);

		// HPƒQ[ƒW‚ð¬‚³‚­•\Ž¦
		hp_gauge.Draw(center_x, center_y_bottom, SMALL);
	}
}
