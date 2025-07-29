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
	UpdateState(); // 影状態の更新

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

void Player::DrawUI()
{
	//if (state == PlayerState::Real)
	//{
	//	DrawRealHPGauge(); // 実態のHPゲージを描画
	//}
	//else
	//{
		DrawCircularShadowGauge(); // 右上に大きく影ゲージ（減少する）
	//}
}

void Player::DrawCircularShadowGauge()
{
	const int screen_width = 1280;
	const int screen_margin = 20;

	const int outer_radius = 60;     // ゲージ外側の半径（拡大）
	const int thickness = 30;        // 厚みを増してドーナツ内側を小さく
	const int inner_radius = outer_radius - thickness;

	// 右上に配置
	const int center_x = screen_width - outer_radius - screen_margin;
	const int center_y = outer_radius + screen_margin;

	// ゲージ割合（0.0～1.0）
	float rate = Clamp(shadow_gauge / shadow_gauge_max, 0.0f, 1.0f);
	float max_angle = 360.0f * rate;

	// --- 背景リング ---
	for (float angle = 0.0f; angle < 360.0f; angle += 2.0f)
	{
		float rad = DX_PI_F * angle / 180.0f;

		int x1 = static_cast<int>(center_x + cosf(rad) * inner_radius);
		int y1 = static_cast<int>(center_y + sinf(rad) * inner_radius);
		int x2 = static_cast<int>(center_x + cosf(rad) * outer_radius);
		int y2 = static_cast<int>(center_y + sinf(rad) * outer_radius);

		DrawLine(x1, y1, x2, y2, GetColor(40, 40, 40)); // 背景（濃いグレー）
	}

	// --- ゲージ部分 ---
	for (float angle = 0.0f; angle < max_angle; angle += 2.0f)
	{
		float rad = DX_PI_F * angle / 180.0f;

		int x1 = static_cast<int>(center_x + cosf(rad) * inner_radius);
		int y1 = static_cast<int>(center_y + sinf(rad) * inner_radius);
		int x2 = static_cast<int>(center_x + cosf(rad) * outer_radius);
		int y2 = static_cast<int>(center_y + sinf(rad) * outer_radius);

		DrawLine(x1, y1, x2, y2, GetColor(180, 80, 255)); // 明るめの紫
	}

	// --- 内側の円（空洞） ---
	DrawCircleAA(center_x, center_y, inner_radius - 1, 64, GetColor(0, 0, 0), TRUE);
}

void Player::DrawRealHPGauge()
{
	const int screen_width = 1280;
	const int screen_margin = 20;

	const int outer_radius = 60;     // ゲージ外側の半径（拡大）
	const int thickness = 30;        // 厚みを増してドーナツ内側を小さく
	const int inner_radius = outer_radius - thickness;
	const float section_angle = 360.0f / 3; // 120度

	// 右上に配置
	const int center_x = screen_width - outer_radius - screen_margin;
	const int center_y = outer_radius + screen_margin;


	// 背景ドーナツ（薄いグレー）
	for (float angle = 0; angle < 360.0f; angle += 2.0f)
	{
		float rad = DX_PI_F * angle / 180.0f;
		int x1 = static_cast<int>(center_x + cosf(rad) * inner_radius);
		int y1 = static_cast<int>(center_y + sinf(rad) * inner_radius);
		int x2 = static_cast<int>(center_x + cosf(rad) * outer_radius);
		int y2 = static_cast<int>(center_y + sinf(rad) * outer_radius);
		DrawLine(x1, y1, x2, y2, GetColor(80, 80, 80));
	}

	// 区切り線（3分割の境界）
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

	// 塗りつぶし部分（HP分だけ区間を塗る）
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
			DrawLine(x1, y1, x2, y2, GetColor(150, 255, 100)); // 紫系
		}
	}
}




