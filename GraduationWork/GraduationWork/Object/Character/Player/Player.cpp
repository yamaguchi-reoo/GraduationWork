#include <DxLib.h>
#include "Player.h"
#include "../../ObjectManager.h"
#include "../../../Utility/UtilityList.h"
#include "../../../common.h"

#include <algorithm>


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
	hp_gauge(),
	attack_hitboxes()
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
	UpdateAttack();

	hp_gauge.SetValue(hp); 

	__super::Update();
}

void Player::Draw(Vector2D offset, double rate)
{

	Vector2D screen_pos = location - offset;
	__super::Draw(offset, rate);

	if (state == PlayerState::Real)
	{
		// 実態の描画
		DrawBoxAA(screen_pos.x, screen_pos.y, screen_pos.x + box_size.x, screen_pos.y + box_size.y, GetColor(255, 0, 0), TRUE);
	}
	else if (state == PlayerState::Shadow)
	{
		// 影状態の描画
		DrawBoxAA(screen_pos.x, screen_pos.y, screen_pos.x + box_size.x, screen_pos.y + box_size.y, GetColor(180, 80, 255), TRUE);
	}

	DrawUI();

#ifdef _DEBUG
	DrawFormatStringF(screen_pos.x, screen_pos.y, GetColor(255, 255, 255), "Player");
	DrawFormatString(0, 40, GetColor(255, 255, 255), "State: %s", (state == PlayerState::Real) ? "Real" : "Shadow");
	//DrawFormatString(0, 60, GetColor(255, 255, 255), "Gauge: %f", shadow_gauge);

	for (const auto& hitbox : attack_hitboxes)
	{
		Vector2D draw_pos = hitbox.position - offset;
		DrawBoxAA(draw_pos.x, draw_pos.y, draw_pos.x + hitbox.size.x, draw_pos.y + hitbox.size.y, GetColor(255, 255, 0), TRUE);
	}

	// Playerの位置
	DrawFormatString(10, 190, GetColor(255, 255, 255), "Player: (%.2f, %.2f)", location.x, location.y);
#endif // DEBUG

}

void Player::Finalize()
{
	__super::Finalize();
}

void Player::OnHitCollision(GameObject* hit_object)
{
	__super::OnHitCollision(hit_object);

	int type = hit_object->GetObjectType();

	if (type == LIGHT && state == PlayerState::Shadow)
	{
		// 影状態で光に当たったら削除
		if (object_manager)
		{
			object_manager->RequestDeleteObject(this);
		}
	}
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
	else
	{
		// 停止
		velocity.x = 0.0f;
	}

	
	//影状態
	if (input->GetButtonDown(XINPUT_BUTTON_RIGHT_SHOULDER))
	{
		// Aボタンで状態を切り替え
		SwitchState();
	}

	if (input->GetButtonDown(XINPUT_BUTTON_A) && !is_jumping && state == PlayerState::Real)
	{
		// ジャンプ
		is_jumping = true;
		on_ground = false;
		jump_velocity = -jump_strength; // ジャンプの初速を設定
	}


	if(input->GetKeyDown(KEY_INPUT_Z) && state == PlayerState::Real)
	{
		hp--;
		if (hp <= 0)
		{
			hp = 0;
			if (object_manager)
			{
				object_manager->RequestDeleteObject(this); // HPが0になったら削除要求
			}
		}
	}

	if (input->GetButtonDown(XINPUT_BUTTON_B))
	{
		if (!is_attacking && attack_cooldown <= 0)
		{
			is_attacking = true;
			attack_cooldown = attack_cooldown_max; // 攻撃クールダウンを設定


			// 攻撃のヒットボックスを追加
			AttackHitBox hitbox;

			hitbox.frame = 0;
			hitbox.size = Vector2D(70, 70); // サイズ設定

			// 向きによって出す位置を変更
			if (flip_flg) {
				// 左向き
				hitbox.position.x = location.x - hitbox.size.x;
			}
			else {
				// 右向き
				hitbox.position.x = location.x + box_size.x;
			}

			// Y座標はプレイヤーの中央に合わせる
			hitbox.position.y = location.y + (box_size.y / 2) - (hitbox.size.y / 2);
			attack_hitboxes.push_back(hitbox);
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
	if (attack_cooldown > 0)
	{
		attack_cooldown--;
	}

	// 攻撃ヒットボックスの位置更新
	for (auto& hitbox : attack_hitboxes)
	{
		hitbox.frame++;

		// プレイヤーの位置に追従させる
		if (flip_flg)
		{
			hitbox.position.x = location.x - hitbox.size.x;
		}
		else
		{
			hitbox.position.x = location.x + box_size.x;
		}
		hitbox.position.y = location.y + (box_size.y / 2) - (hitbox.size.y / 2);
	}

	// frame > 5 のものを削除
	attack_hitboxes.erase(
		std::remove_if(attack_hitboxes.begin(), attack_hitboxes.end(),
			[](const AttackHitBox& h) { return h.frame > 10; }),
		attack_hitboxes.end()
	);

	// 攻撃判定がなくなったら攻撃状態を解除して再度攻撃できるようにする
	if (attack_hitboxes.empty())
	{
		is_attacking = false;
	}
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
	// 影状態のときは影ゲージを更新
	if (state == PlayerState::Shadow)
	{
		// 影ゲージを消費
		shadow_gauge.Update(true,8);
		if (shadow_gauge.IsEmpty()) 
		{
			SwitchState();
		}
	}
	else if (state == PlayerState::Real)
	{
		// 実態のときは影ゲージを回復
		shadow_gauge.Update(false,8);
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
