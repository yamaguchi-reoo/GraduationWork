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
		shadow_gauge -= shadow_consumption; // �e���Q�[�W������
		if (shadow_gauge < 0.0f)
		{
			shadow_gauge = 0.0f; // �Q�[�W��0�����ɂȂ�Ȃ��悤�ɂ���

			state = PlayerState::Real; // �Q�[�W��0�ɂȂ�������Ԃɖ߂�
		}
	}
	else
	{
		shadow_gauge += shadow_consumption * 0.1f; // �Q�[�W�����X�ɉ�
		if (shadow_gauge > shadow_gauge_max)
		{
			shadow_gauge = shadow_gauge_max; // �ő�l�𒴂��Ȃ��悤�ɂ���
		}
	}

}

void Player::DrawUI()
{
	//if (state == PlayerState::Real)
	//{
	//	DrawRealHPGauge(); // ���Ԃ�HP�Q�[�W��`��
	//}
	//else
	//{
		DrawCircularShadowGauge(); // �E��ɑ傫���e�Q�[�W�i��������j
	//}
}

void Player::DrawCircularShadowGauge()
{
	const int screen_width = 1280;
	const int screen_margin = 20;

	const int outer_radius = 60;     // �Q�[�W�O���̔��a�i�g��j
	const int thickness = 30;        // ���݂𑝂��ăh�[�i�c������������
	const int inner_radius = outer_radius - thickness;

	// �E��ɔz�u
	const int center_x = screen_width - outer_radius - screen_margin;
	const int center_y = outer_radius + screen_margin;

	// �Q�[�W�����i0.0�`1.0�j
	float rate = Clamp(shadow_gauge / shadow_gauge_max, 0.0f, 1.0f);
	float max_angle = 360.0f * rate;

	// --- �w�i�����O ---
	for (float angle = 0.0f; angle < 360.0f; angle += 2.0f)
	{
		float rad = DX_PI_F * angle / 180.0f;

		int x1 = static_cast<int>(center_x + cosf(rad) * inner_radius);
		int y1 = static_cast<int>(center_y + sinf(rad) * inner_radius);
		int x2 = static_cast<int>(center_x + cosf(rad) * outer_radius);
		int y2 = static_cast<int>(center_y + sinf(rad) * outer_radius);

		DrawLine(x1, y1, x2, y2, GetColor(40, 40, 40)); // �w�i�i�Z���O���[�j
	}

	// --- �Q�[�W���� ---
	for (float angle = 0.0f; angle < max_angle; angle += 2.0f)
	{
		float rad = DX_PI_F * angle / 180.0f;

		int x1 = static_cast<int>(center_x + cosf(rad) * inner_radius);
		int y1 = static_cast<int>(center_y + sinf(rad) * inner_radius);
		int x2 = static_cast<int>(center_x + cosf(rad) * outer_radius);
		int y2 = static_cast<int>(center_y + sinf(rad) * outer_radius);

		DrawLine(x1, y1, x2, y2, GetColor(180, 80, 255)); // ����߂̎�
	}

	// --- �����̉~�i�󓴁j ---
	DrawCircleAA(center_x, center_y, inner_radius - 1, 64, GetColor(0, 0, 0), TRUE);
}

void Player::DrawRealHPGauge()
{
	const int screen_width = 1280;
	const int screen_margin = 20;

	const int outer_radius = 60;     // �Q�[�W�O���̔��a�i�g��j
	const int thickness = 30;        // ���݂𑝂��ăh�[�i�c������������
	const int inner_radius = outer_radius - thickness;
	const float section_angle = 360.0f / 3; // 120�x

	// �E��ɔz�u
	const int center_x = screen_width - outer_radius - screen_margin;
	const int center_y = outer_radius + screen_margin;


	// �w�i�h�[�i�c�i�����O���[�j
	for (float angle = 0; angle < 360.0f; angle += 2.0f)
	{
		float rad = DX_PI_F * angle / 180.0f;
		int x1 = static_cast<int>(center_x + cosf(rad) * inner_radius);
		int y1 = static_cast<int>(center_y + sinf(rad) * inner_radius);
		int x2 = static_cast<int>(center_x + cosf(rad) * outer_radius);
		int y2 = static_cast<int>(center_y + sinf(rad) * outer_radius);
		DrawLine(x1, y1, x2, y2, GetColor(80, 80, 80));
	}

	// ��؂���i3�����̋��E�j
	for (int i = 0; i < 3; ++i)
	{
		float angle_deg = i * section_angle;
		float rad = DX_PI_F * angle_deg / 180.0f;
		int x1 = static_cast<int>(center_x + cosf(rad) * inner_radius);
		int y1 = static_cast<int>(center_y + sinf(rad) * inner_radius);
		int x2 = static_cast<int>(center_x + cosf(rad) * outer_radius);
		int y2 = static_cast<int>(center_y + sinf(rad) * outer_radius);
		DrawLine(x1, y1, x2, y2, GetColor(120, 120, 120));
	}

	// �h��Ԃ������iHP��������Ԃ�h��j
	for (int i = 0; i < hp; ++i)
	{
		float start_angle = i * section_angle;
		float end_angle = start_angle + section_angle;

		for (float angle = start_angle; angle < end_angle; angle += 2.0f)
		{
			float rad = DX_PI_F * angle / 180.0f;
			int x1 = static_cast<int>(center_x + cosf(rad) * inner_radius);
			int y1 = static_cast<int>(center_y + sinf(rad) * inner_radius);
			int x2 = static_cast<int>(center_x + cosf(rad) * outer_radius);
			int y2 = static_cast<int>(center_y + sinf(rad) * outer_radius);
			DrawLine(x1, y1, x2, y2, GetColor(150, 255, 100)); // ���n
		}
	}
}




