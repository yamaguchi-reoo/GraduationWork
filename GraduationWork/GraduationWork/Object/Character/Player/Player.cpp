#include <DxLib.h>
#include "Player.h"
#include "../Enemy/EnemyBase.h"
#include "../Enemy/Enemy.h"
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
	jump_strength(12.0f),
	is_attacking(false),
	invincible_timer(0),
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

	hp = 3;

	is_jumping = false;

	shadow_gauge.Initialize(GaugeType::CircularFill, 1200, 1200, 0, GetColor(180, 80, 255));
	hp_gauge.Initialize(GaugeType::CircularSection, 3, hp, 3, GetColor(255, 0, 0));

	LoadPlayerImage();
}

void Player::Update()
{
	HandleInput();
	UpdateState(); 
	//UpdateJump(); 
	UpdateAttack();
	UpdateAnimation();

	hp_gauge.SetValue(hp); 

	// ���G�^�C�}�[����
	if (invincible_timer > 0) {
		invincible_timer--;
	}

	__super::Update();
}

void Player::Draw(Vector2D offset, double rate)
{

	Vector2D screen_pos = location - offset;
	//__super::Draw(offset, 3.0f);
	
	bool visible = true;
	if (invincible_timer > 0)
	{
		visible = (invincible_timer / 5) % 2 == 0; // �_�Ō���
	}

	if (visible) {
		if (state == PlayerState::Real)
		{
			// ���Ԃ̕`��
			DrawBoxAA(screen_pos.x, screen_pos.y, screen_pos.x + box_size.x, screen_pos.y + box_size.y, GetColor(255, 0, 0), FALSE);
		}
		else if (state == PlayerState::Shadow)
		{
			// �e��Ԃ̕`��
			DrawBoxAA(screen_pos.x, screen_pos.y, screen_pos.x + box_size.x, screen_pos.y + box_size.y, GetColor(180, 80, 255), FALSE);

			float offset_x = (flip_flg ? -100.0f : 100.0f);
			DrawRotaGraphF(screen_pos.x + (box_size.x / 2) + offset_x, screen_pos.y + (box_size.y / 2) - 8, 3.0f, 0.0, image, TRUE, flip_flg);
		}
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

	if (type == ENEMY || type == REALENEMY)
	{
		// �G�ɓ���������_���[�W
		if (state == PlayerState::Real && invincible_timer <= 0)
		{
			if (object_manager)
			{
				hp--;
				invincible_timer = 60;
				if (hp <= 0)
				{
					hp = 0;
					if (object_manager)
					{
						object_manager->RequestDeleteObject(this); // HP��0�ɂȂ�����폜�v��
					}
				}
			}
		}

	}
}

void Player::HandleInput()
{
	InputManager* input = InputManager::GetInstance();

	// �����ړ��i�����x����j
	float target_speed = 0.0f;
	if (input->GetButton(XINPUT_BUTTON_DPAD_LEFT)) {
		target_speed = -3.5f;
		flip_flg = TRUE;
	}
	else if (input->GetButton(XINPUT_BUTTON_DPAD_RIGHT)) {
		target_speed = 3.5f;
		flip_flg = FALSE;
	}

	const float accel = 0.5f; // ���x�ω��̊����i0�`1�j
	velocity.x += (target_speed - velocity.x) * accel;

	// ��Ԑؑ�
	if (input->GetButtonDown(XINPUT_BUTTON_RIGHT_SHOULDER))
	{
		// A�{�^���ŏ�Ԃ�؂�ւ�
		SwitchState();
	}


	// �W�����v
	if (input->GetButtonDown(XINPUT_BUTTON_A) && !is_jumping && state == PlayerState::Real)
	{
		is_jumping = true;
		on_ground = false;
		velocity.y = -jump_strength;
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

	// === �G�Ƃ̓����蔻�� ===
	if (!attack_hitboxes.empty() && object_manager)
	{
		for (auto enemy_obj : object_manager->GetObjects(ENEMY)) 
		{
			int type = enemy_obj->GetObjectType();
			if (type == ENEMY || type == REALENEMY)
			{
				Enemy* enemy = dynamic_cast<Enemy*>(enemy_obj);
				//EnemyBase* enemy = dynamic_cast<EnemyBase*>(enemy_obj);
				if (!enemy) continue; // �e��Ԃ̓G�͖���

				for (const auto& hitbox : attack_hitboxes)
				{
					// �����ŋ�`���m�̓����蔻�������
					bool hit = (
						hitbox.position.x < enemy->GetLocation().x + enemy->GetBoxSize().x &&
						hitbox.position.x + hitbox.size.x > enemy->GetLocation().x &&
						hitbox.position.y < enemy->GetLocation().y + enemy->GetBoxSize().y &&
						hitbox.position.y + hitbox.size.y > enemy->GetLocation().y
						);
					if (hit)
					{
						//enemy->ReceiveDamage(1); // �G�Ƀ_���[�W��^����
						object_manager->RequestDeleteObject(enemy); // �G���폜
						break;
					}
				}
			}
		}
	}

	// frame > 10 �̂��̂��폜
	attack_hitboxes.erase(
		std::remove_if(attack_hitboxes.begin(), attack_hitboxes.end(),
			[](const AttackHitBox& h) { return h.frame > 10; }),
		attack_hitboxes.end()
	);

	if (attack_hitboxes.empty())
	{
		is_attacking = false;
	}
}

void Player::UpdateAnimation()
{
	if (animation_data.empty()) return;

	// �t���[���^�C�}�[��i�߂�
	frame_timer++;
	if (frame_timer >= frame_delay)
	{
		frame_timer = 0;
		current_frame++;
		if (current_frame >= animation_data.size())
			current_frame = 0; // ���[�v

		// �\���p�摜���X�V
		image = animation_data[current_frame];
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

void Player::LoadPlayerImage()
{
	// ResourceManager �̃C���X�^���X�擾
	ResourceManager* rm = ResourceManager::GetInstance();

	// �摜�擾�i�P�̉摜�̏ꍇ all_num = 1 �Ȃ̂ŏȗ��\�j
	auto idle_image = rm->GetImages("Resource/Images/Character/Player/Player_idle.png", 7, 7, 1, 128, 70);
	//auto idle_image = rm->GetImages("Resource/Images/Character/Player/Player_idle.png",1);

	// �A�j���[�V�����p�ɃR�s�[�i����͒P�̉摜�����ǔz�񈵂��j
	animation_data = idle_image;

	// �\���p�摜�ɐݒ�
	image = animation_data[0];

	// �A�j���[�V�����֘A�ϐ�������
	current_frame = 0;
	frame_timer = 0;
	frame_delay = 5; // 5�t���[�����ƂɎ��̉摜�ɐ؂�ւ���
}
