#include "Plate.h"
#include "DxLib.h"
#include "../../Utility/SoundManager.h"
#include "../../Utility/UserTemplate.h"

namespace
{
	constexpr float PLATE_HEIGHT = 3.0f;
	constexpr float PRESS_DEPTH = 6.0f;   // 最大で沈む量
	constexpr float PRESS_SPEED = 0.5f;   // 沈む＆戻る速度
	constexpr float PLATE_Y_OFFSET = 40.0f;
}

void Plate::Initialize(Vector2D _location, Vector2D _box_size)
{
	object_type = PLATE;
	__super::Initialize(_location, _box_size);

	TrapOn_flg = false;
	prev_TrapOn_flg = false;

	press_offset = 0.0f;
	press_offset_target = 0.0f;
}

void Plate::Update()
{
	// OFF → ON の瞬間だけSE
	if (!prev_TrapOn_flg && TrapOn_flg)
	{
		SoundManager::GetInstance()->Play(SoundID::PLATE_ON);
	}

	// ライト制御
	if (linked_light)
	{
		linked_light->SetLightMoving(TrapOn_flg);
	}

	// 目標沈み量を決定
	press_offset_target = TrapOn_flg ? PRESS_DEPTH : 0.0f;

	// 補間（なめらかに）
	if (press_offset < press_offset_target)
	{
		press_offset = Min(press_offset + PRESS_SPEED, press_offset_target);
	}
	else if (press_offset > press_offset_target)
	{
		press_offset = Max(press_offset - PRESS_SPEED, press_offset_target);
	}

	prev_TrapOn_flg = TrapOn_flg;

	__super::Update();

	TrapOn_flg = false;
}

void Plate::Draw(Vector2D offset, double rate)
{
#ifdef _DEBUG
	Vector2D screen_pos = location - offset;

	const float y = screen_pos.y + PLATE_Y_OFFSET + press_offset;

	const int fillColor = GetColor(125, 125, 125); // 本体
	const int borderColor = GetColor(175, 175, 175);    // 輪郭（濃いグレー）

	// 輪郭（少し大きめ）
	DrawBoxAA(
		screen_pos.x - 1,
		y - 1,
		screen_pos.x + box_size.x + 1,
		y + PLATE_HEIGHT + 6,
		borderColor,
		FALSE
	);

	// 本体
	DrawBoxAA(
		screen_pos.x,
		y,
		screen_pos.x + box_size.x,
		y + PLATE_HEIGHT + 5,
		fillColor,
		TRUE
	);
#endif
}

void Plate::Finalize()
{
	__super::Finalize();
}

void Plate::OnHitCollision(GameObject* hit_object)
{
	const int type = hit_object->GetObjectType();

	if (type != PLAYER && type != PUSHBLOCK) return;

	if (IsOnPlate(hit_object))
	{
		TrapOn_flg = true;
	}
}

bool Plate::IsOnPlate(GameObject* obj) const
{
	const float yOffset = box_size.y - PLATE_HEIGHT;

	const Vector2D pos = obj->GetLocation();
	const Vector2D size = obj->GetBoxSize();

	return !(
		pos.x + size.x < location.x ||
		pos.x > location.x + box_size.x ||
		pos.y + size.y < location.y + yOffset ||
		pos.y > location.y + yOffset + PLATE_HEIGHT
		);
}

