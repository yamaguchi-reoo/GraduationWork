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
		// ���Ԃ̕`��
		DrawBoxAA(screen_pos.x, screen_pos.y, screen_pos.x + box_size.x, screen_pos.y + box_size.y, GetColor(255, 0, 0), TRUE);
	}
	else if (state == PlayerState::Shadow)
	{
		// �e��Ԃ̕`��
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

	// Player�̈ʒu
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
		// �e��ԂŌ��ɓ���������폜
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

	
	//�e���
	if (input->GetButtonDown(XINPUT_BUTTON_RIGHT_SHOULDER))
	{
		// A�{�^���ŏ�Ԃ�؂�ւ�
		SwitchState();
	}

	if (input->GetButtonDown(XINPUT_BUTTON_A) && !is_jumping && state == PlayerState::Real)
	{
		// �W�����v
		is_jumping = true;
		on_ground = false;
		jump_velocity = -jump_strength; // �W�����v�̏�����ݒ�
	}


	if(input->GetKeyDown(KEY_INPUT_Z) && state == PlayerState::Real)
	{
		hp--;
		if (hp <= 0)
		{
			hp = 0;
			if (object_manager)
			{
				object_manager->RequestDeleteObject(this); // HP��0�ɂȂ�����폜�v��
			}
		}
	}

	if (input->GetButtonDown(XINPUT_BUTTON_B))
	{
		if (!is_attacking && attack_cooldown <= 0)
		{
			is_attacking = true;
			attack_cooldown = attack_cooldown_max; // �U���N�[���_�E����ݒ�


			// �U���̃q�b�g�{�b�N�X��ǉ�
			AttackHitBox hitbox;

			hitbox.frame = 0;
			hitbox.size = Vector2D(70, 70); // �T�C�Y�ݒ�

			// �����ɂ���ďo���ʒu��ύX
			if (flip_flg) {
				// ������
				hitbox.position.x = location.x - hitbox.size.x;
			}
			else {
				// �E����
				hitbox.position.x = location.x + box_size.x;
			}

			// Y���W�̓v���C���[�̒����ɍ��킹��
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

	// �U���q�b�g�{�b�N�X�̈ʒu�X�V
	for (auto& hitbox : attack_hitboxes)
	{
		hitbox.frame++;

		// �v���C���[�̈ʒu�ɒǏ]������
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

	// frame > 5 �̂��̂��폜
	attack_hitboxes.erase(
		std::remove_if(attack_hitboxes.begin(), attack_hitboxes.end(),
			[](const AttackHitBox& h) { return h.frame > 10; }),
		attack_hitboxes.end()
	);

	// �U�����肪�Ȃ��Ȃ�����U����Ԃ��������čēx�U���ł���悤�ɂ���
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
	// �e��Ԃ̂Ƃ��͉e�Q�[�W���X�V
	if (state == PlayerState::Shadow)
	{
		// �e�Q�[�W������
		shadow_gauge.Update(true,8);
		if (shadow_gauge.IsEmpty()) 
		{
			SwitchState();
		}
	}
	else if (state == PlayerState::Real)
	{
		// ���Ԃ̂Ƃ��͉e�Q�[�W����
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
		// HP�Q�[�W�i�Z�N�V���������j
		hp_gauge.Draw(center_x, center_y_top, LARGE);

		// �e�Q�[�W�i�A���h��Ԃ��j
		shadow_gauge.Draw(center_x, center_y_bottom, SMALL);
	}
	else
	{
		// �e��Ԃ̂Ƃ��e�Q�[�W��傫���\��
		shadow_gauge.Draw(center_x, center_y_top, LARGE);

		// HP�Q�[�W���������\��
		hp_gauge.Draw(center_x, center_y_bottom, SMALL);
	}
}
