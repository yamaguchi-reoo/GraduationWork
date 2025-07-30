#include <DxLib.h>
#include "Player.h"
#include "../../../Utility/UtilityList.h"
#include "../../../common.h"

namespace {
	constexpr int BASE_RADIUS = 60;
	constexpr int BASE_THICKNESS = 30;
}

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

	hp = 3;
}

void Player::Update()
{
	HandleInput();
	UpdateState(); // �e��Ԃ̍X�V

	__super::Update();
}

void Player::Draw(Vector2D offset, double rate)
{
	__super::Draw(offset, rate);

	if (state == PlayerState::Real)
	{
		// ���Ԃ̕`��
		DrawBoxAA(offset.x, offset.y, offset.x + box_size.x, offset.y + box_size.y, GetColor(255, 0, 0), TRUE);
	}
	else if (state == PlayerState::Shadow)
	{
		// �e��Ԃ̕`��
		DrawBoxAA(offset.x, offset.y, offset.x + box_size.x, offset.y + box_size.y, GetColor(180, 80, 255), TRUE);
	}

	DrawUI();

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
	else if (input->GetButtonDown(XINPUT_BUTTON_RIGHT_SHOULDER))
	{
		// A�{�^���ŏ�Ԃ�؂�ւ�
		SwitchState();
	}
	else
	{
		// ��~
		velocity.x = 0.0f;
	}
}

void Player::SwitchState()
{
	if (state == PlayerState::Real)
	{
		// �e��ԂɈڍs�ł������
		if (shadow_gauge >= shadow_consumption * 10.0f)
		{
			state = PlayerState::Shadow;
		}
	}
	else if (state == PlayerState::Shadow)
	{
		// ���Ԃɖ߂�
		state = PlayerState::Real;
	}
}

void Player::UpdateState()
{
	if (state == PlayerState::Shadow)
	{
		// �Q�[�W������
		shadow_gauge -= shadow_consumption;
		if (shadow_gauge <= 0.0f)
		{
			shadow_gauge = 0.0f;
			// �Q�[�W���s�����̂Ŏ��Ԃɖ߂�
			SwitchState();
		}
	}
	else if (state == PlayerState::Real)
	{
		// ���Ԏ��̓Q�[�W�������ŉ�
		shadow_gauge += 0.5f; // �񕜗ʂ͒����\
		if (shadow_gauge > shadow_gauge_max)
		{
			shadow_gauge = shadow_gauge_max;
		}
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
		DrawRealHPGauge(center_x, center_y_top, LARGE);
		DrawCircularShadowGauge(center_x, center_y_bottom, SMALL);
	}
	else
	{
		DrawCircularShadowGauge(center_x, center_y_top, LARGE);
		DrawRealHPGauge(center_x, center_y_bottom, SMALL);
	}
}

void Player::DrawCircularShadowGauge(int center_x, int center_y, float scale)
{
	int outer_radius = static_cast<int>(BASE_RADIUS * scale);
	int inner_radius = outer_radius - static_cast<int>(BASE_THICKNESS * scale);

	float fill_rate = Clamp(shadow_gauge / shadow_gauge_max, 0.0f, 1.0f);
	float fill_angle = 360.0f * fill_rate;

	DrawArc(center_x, center_y, inner_radius, outer_radius, 0, 360, GetColor(40, 40, 40));        // �w�i
	DrawArc(center_x, center_y, inner_radius, outer_radius, 0, fill_angle, GetColor(180, 80, 255)); // ���Q�[�W

	DrawCircleAA(center_x, center_y, inner_radius - 1, 64, GetColor(0, 0, 0), TRUE); // ������
}

void Player::DrawRealHPGauge(int center_x, int center_y, float scale)
{
	constexpr int SECTION_COUNT = 3;

	int outer_radius = static_cast<int>(BASE_RADIUS * scale);
	int inner_radius = outer_radius - static_cast<int>(BASE_THICKNESS * scale);
	float section_angle = 360.0f / SECTION_COUNT;

	DrawArc(center_x, center_y, inner_radius, outer_radius, 0, 360, GetColor(80, 80, 80)); // �w�i

	for (int i = 0; i < SECTION_COUNT; ++i)
	{
		float angle = i * section_angle;
		DrawArc(center_x, center_y, inner_radius, outer_radius, angle, angle + 0.5f, GetColor(120, 120, 120)); // ��؂��
	}

	for (int i = 0; i < hp; ++i)
	{
		float start_angle = i * section_angle;
		DrawArc(center_x, center_y, inner_radius, outer_radius, start_angle, start_angle + section_angle, GetColor(150, 255, 100)); // �u����
	}
}

void Player::DrawArc(int center_x, int center_y, int inner_radius, int outer_radius, float start_angle, float end_angle, int color)
{
	for (float angle = start_angle; angle < end_angle; angle += 2.0f)
	{
		float rad = DX_PI_F * angle / 180.0f;
		int x1 = center_x + cosf(rad) * inner_radius;
		int y1 = center_y + sinf(rad) * inner_radius;
		int x2 = center_x + cosf(rad) * outer_radius;
		int y2 = center_y + sinf(rad) * outer_radius;
		DrawLine(x1, y1, x2, y2, color);
	}
}


