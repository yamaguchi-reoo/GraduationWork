#include "CharacterBase.h"

void CharacterBase::Initialize(Vector2D _location, Vector2D _box_size)
{
	__super::Initialize(_location, _box_size);
	on_ground = false;
}

void CharacterBase::Update()
{
	__super::Update();
}

void CharacterBase::Draw(Vector2D offset, double rate)
{
	__super::Draw(offset, rate);
}

void CharacterBase::Finalize()
{
	__super::Finalize();
}

void CharacterBase::OnHitCollision(GameObject* hit_object)
{

}