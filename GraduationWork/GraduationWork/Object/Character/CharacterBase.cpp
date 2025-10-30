#include "CharacterBase.h"
#include "../Gimmick/PushBlock.h"

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
	Vector2D screen_pos = location - offset;
	__super::Draw(screen_pos, rate);
}

void CharacterBase::Finalize()
{
	__super::Finalize();
}

void CharacterBase::OnHitCollision(GameObject* hit_object)
{
    int type = hit_object->GetObjectType();

    // �e��ԂȂ�u�ǁv���u������u���b�N�v�����蔲����
    if (IsPlayerShadow() && (type == WALL || type == PUSHBLOCK)) return;

    // �Ώۂ̓u���b�N or ��
    if (type != BLOCK && type != WALL && type != PUSHBLOCK && type != INVISIBLEFLOOR) return;

    // AABB���擾�i�����j
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

    // �e��������̉����o����
    float from_top = other_bottom - my_top;
    float from_bottom = my_bottom - other_top;
    float from_left = other_right - my_left;
    float from_right = my_right - other_left;

    float min_y = from_top < from_bottom ? from_top : from_bottom;
    float min_x = from_left < from_right ? from_left : from_right;

    // �D����������߂�i���x���傫��������D��j
    bool vertical_first = (fabsf(velocity.y) > fabsf(velocity.x) + 0.1f);

    // �����I�t�Z�b�g
    const float epsilon = 0.01f;

    if (vertical_first && min_y < min_x)
    {
        // �����D��
        if (from_top < from_bottom)
        {
            // �ォ�牟���Ԃ����
            location.y = other_bottom + epsilon;
            if (velocity.y < 0) velocity.y = 0;
        }
        else
        {
            // �����璅�n
            location.y = other_top - my_size.y;
            velocity.y = 0;
            on_ground = true;
            is_jumping = false;
        }

        // �ق�̏������������炷
        if (velocity.x > 0) location.x -= epsilon;
        else if (velocity.x < 0) location.x += epsilon;
    }
    else if (!vertical_first && min_x < min_y)
    {
        // �����D��
        if (from_left < from_right)
        {
            // �����牟���Ԃ����
            location.x = other_right;
            velocity.x = 0;

            if (type == PUSHBLOCK)
            {
                PushBlock* pb = dynamic_cast<PushBlock*>(hit_object);
                if (pb) pb->StartMove(Vector2D(-1.0f, 0.0f));
            }
        }
        else
        {
            // �E���牟���Ԃ����
            location.x = other_left - my_size.x;
            velocity.x = 0;

            if (type == PUSHBLOCK)
            {
                PushBlock* pb = dynamic_cast<PushBlock*>(hit_object);
                if (pb) pb->StartMove(Vector2D(1.0f, 0.0f));
            }
        }

        // �ق�̏������������炷
        if (velocity.y > 0) location.y -= epsilon;
        else if (velocity.y < 0) location.y += epsilon;
    }
    else
    {
        // �ǂ���ł��������ŏd�Ȃ������
        if (min_y < min_x)
        {
            if (from_top < from_bottom)
                location.y = other_bottom + epsilon;
            else
            {
                location.y = other_top - my_size.y;
                velocity.y = 0;
                on_ground = true;
                is_jumping = false;
            }
        }
        else
        {
            if (from_left < from_right)
                location.x = other_right;
            else
                location.x = other_left - my_size.x;

            velocity.x = 0;
        }
    }
}

