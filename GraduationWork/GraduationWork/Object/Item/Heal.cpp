#include "Heal.h"
#include "../Character/Player/Player.h"
#include "../ObjectManager.h"

void Heal::Initialize(Vector2D _location, Vector2D _box_size)
{
	object_type = HEAL;

	__super::Initialize(_location, _box_size);
}

void Heal::Update()
{
	__super::Update();
}

void Heal::Draw(Vector2D offset, double rate)
{


	Vector2D screen_pos = location - offset;

#ifdef _DEBUG
	DrawBoxAA(screen_pos.x, screen_pos.y, screen_pos.x + box_size.x, screen_pos.y + box_size.y, GetColor(0, 125, 125), TRUE);
#endif
	__super::Draw(screen_pos, rate);


}


void Heal::Finalize()
{
	__super::Finalize();
}

void Heal::OnHitCollision(GameObject* hit_object)
{
	int type = hit_object->GetObjectType();
	if (type == PLAYER) {
		Player* player = dynamic_cast<Player*>(hit_object);

		player->AddHP(1);
		object_manager->RequestDeleteObject(this);
	}
}
