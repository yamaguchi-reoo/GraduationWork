#include "CharacterBase.h"

void CharacterBase::Initialize(Vector2D _location, Vector2D _box_size)
{
	__super::Initialize(_location, _box_size);
	on_ground = false;
}

void CharacterBase::Update()
{
    on_ground = false;

    // �n�ʂɂ��Ȃ��Ƃ�������������
    if (!on_ground)
    {
        velocity.y += gravity;

        // �ő嗎�����x�𒴂��Ȃ��悤�ɐ���
        if (velocity.y > max_fall_speed)
            velocity.y = max_fall_speed;
    }

    // �ʒu�̍X�V
    location.y += velocity.y;
	location.x += velocity.x; 

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
	// �u���b�N�Ƃ̂ݏ���
	if (hit_object->GetObjectType() != BLOCK) return;

	// AABB���擾�i���W�͍����Ɖ���j
	Vector2D my_pos = GetLocation();
	Vector2D my_size = GetBoxSize();
	Vector2D other_pos = hit_object->GetLocation();
	Vector2D other_size = hit_object->GetBoxSize();

	// �e��
	float my_bottom = my_pos.y + my_size.y;
	float my_top = my_pos.y;
	float my_left = my_pos.x;
	float my_right = my_pos.x + my_size.x;

	float other_bottom = other_pos.y + other_size.y;
	float other_top = other_pos.y;
	float other_left = other_pos.x;
	float other_right = other_pos.x + other_size.x;

	// �e��������̉����o���ʂ��v�Z
	float from_top = other_bottom - my_top;
	float from_bottom = my_bottom - other_top;
	float from_left = other_right - my_left;
	float from_right = my_right - other_left;

	// �ŏ��̉����o�������𔻒�
	float min_y = std::min(from_top, from_bottom);
	float min_x = std::min(from_left, from_right);

	if (min_y < min_x) {
		// ���������̏Փ�
		if (from_top < from_bottom) {
			// �u���b�N�̏�ɒ��n
			location.y = other_bottom;
			velocity.y = 0;
		}
		else {
			// �n�ʂɒ��n
			location.y = other_top - my_size.y;
			velocity.y = 0;
			on_ground = true;
		}
	}
	else {
		// ���������̏Փ�
		if (from_left < from_right) {
			// ������Ԃ�����
			location.x = other_right;
			velocity.x = 0;
		}
		else {
			// �E����Ԃ�����
			location.x = other_left - my_size.x;
			velocity.x = 0;
		}
	}
}
