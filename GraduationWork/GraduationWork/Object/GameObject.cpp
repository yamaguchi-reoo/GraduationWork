#include "DxLib.h"
#include "GameObject.h"
#include <math.h>
#include "../common.h"

GameObject::GameObject() : location(), box_size(), hit_box(), image(NULL), object_type(), flip_flg(FALSE)
{
}

GameObject::~GameObject()
{
}

void GameObject::Initialize(Vector2D _location, Vector2D _box_size)
{
	//�ʒu��ݒ�
	location = _location;
	//�T�C�Y��ݒ�
	box_size = _box_size;

	hit_box = Vector2D(box_size.x, box_size.y);

	//�摜��ݒ�
	//image = NULL;
}

void GameObject::Update()
{
	location += velocity;
}

void GameObject::Draw(Vector2D offset, double rate)
{
	// �`��͈͂̔���
	Vector2D screen_pos = location + offset;
	if (screen_pos.x + box_size.x >= 0 && screen_pos.x < SCREEN_WIDTH)
	{
		//if (image == NULL) return; // �摜���ݒ肳��Ă��Ȃ��ꍇ�͕`�悵�Ȃ�
		// �摜��`��i���S���W�w��j
		DrawRotaGraphF(offset.x + (box_size.x / 2), offset.y + (box_size.y / 2), rate, 0.0, image, TRUE, flip_flg);

	}

#ifdef _DEBUG
	// �q�b�g�{�b�N�X�̒��S���W
	Vector2D center = location + hit_box / 2.0f;

	// ����ƉE�����W���v�Z�i�I�t�Z�b�g�͉����Ȃ��j
	Vector2D top_left = center - hit_box / 2.0f;
	Vector2D bottom_right = center + hit_box / 2.0f;

	// �g�������ԐF�ŕ`��
	DrawBoxAA(top_left.x, top_left.y, bottom_right.x, bottom_right.y, GetColor(255, 0, 0), FALSE);
#endif // _DEBUG
}

void GameObject::Finalize()
{
}

void GameObject::SetLocation(Vector2D _location)
{
	this->location = _location;
}

Vector2D GameObject::GetLocation() const
{
	return this->location;
}

Vector2D GameObject::GetBoxSize() const
{
	return this->box_size;
}

Vector2D GameObject::GetVelocity() const
{
	return this->velocity;
}


eObjectType GameObject::GetObjectType()
{
	return this->object_type;
}

void GameObject::OnHitCollision(GameObject* hit_object)
{

}


bool GameObject::CheckBoxCollision(GameObject* obj)
{
	//�����̍�����W
	Vector2D my_pos = location;
	//�����̕��ƍ����̔���
	Vector2D my_size = hit_box / 2.0f;

	//����̍�����W
	Vector2D sub_pos = obj->GetLocation();
	//����̕��ƍ����̔���
	Vector2D sub_size = obj->GetBoxSize() / 2.0f;

	//���S���W�̍���
	Vector2D diff = (my_pos + my_size) - (sub_pos + sub_size);

	//�����蔻��̉��Z
	return (fabsf(diff.x) <= my_size.x + sub_size.x &&
		fabsf(diff.y) <= my_size.y + sub_size.y);

}
