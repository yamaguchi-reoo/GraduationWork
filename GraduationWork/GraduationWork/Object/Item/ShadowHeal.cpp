#include "ShadowHeal.h"
#include "../Character/Player/Player.h"
#include "../Character/Player/Gauge.h"
#include "../ObjectManager.h"

void ShadowHeal::Initialize(Vector2D _location, Vector2D _box_size)
{
	object_type = SHADOWHEAL;

	__super::Initialize(_location, _box_size);
}

void ShadowHeal::Update()
{
	__super::Update();
}

void ShadowHeal::Draw(Vector2D offset, double rate)
{


	Vector2D screen_pos = location - offset;

#ifdef _DEBUG
	DrawBoxAA(screen_pos.x, screen_pos.y, screen_pos.x + box_size.x, screen_pos.y + box_size.y, GetColor(0, 255, 125), TRUE);
#endif
	__super::Draw(screen_pos, rate);


}


void ShadowHeal::Finalize()
{
	__super::Finalize();
}



void ShadowHeal::OnHitCollision(GameObject* hit_object)
{
	const int GAUGE_RECOVER_AMOUNT = 200;

    int type = hit_object->GetObjectType();
	if (type == PLAYER) {
		Player* player = dynamic_cast<Player*>(hit_object);

		if (player) {

			DrawFormatString(10, 200, GetColor(255, 0, 0), "Gauge HEAL SUCCESS! Value: %d", GAUGE_RECOVER_AMOUNT);

			// 1. �v���C���[����Q�[�W�I�u�W�F�N�g�̎Q�Ƃ��擾 (Player::GetGauge()�͕K�v)
			Gauge& player_gauge = player->GetGauge();

			// 2. �y�V�������J���\�b�h AddValue() �ŉ񕜗ʂ𒼐ړn���z
			player_gauge.AddValue(GAUGE_RECOVER_AMOUNT);

			// �A�C�e�����폜
			object_manager->RequestDeleteObject(this);
		}
	}
}
