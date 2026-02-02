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
	//位置を設定
	location = _location;
	//サイズを設定
	box_size = _box_size;

	hit_box = Vector2D(box_size.x, box_size.y);

	//画像を設定
	//image = NULL;
}

void GameObject::Update()
{
	location += velocity;
}

void GameObject::Draw(Vector2D offset, double rate)
{
	// 描画範囲の判定
	Vector2D screen_pos = location - offset;
	if (screen_pos.x + box_size.x >= 0 && screen_pos.x < SCREEN_WIDTH)
	{
		//if (image == NULL) return; // 画像が設定されていない場合は描画しない
		// 画像を描画（中心座標指定）
		DrawRotaGraphF(offset.x + (box_size.x / 2), offset.y + (box_size.y / 2), rate, 0.0, image, TRUE, flip_flg);

	}

//#ifdef _DEBUG
	//DrawBoxAA(offset.x, offset.y, offset.x + box_size.x, offset.y + box_size.y, GetColor(255, 0, 0), FALSE);
//#endif // _DEBUG
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
    Vector2D my_center = location + hit_box / 2.0f;
    Vector2D obj_center = obj->GetLocation() + obj->GetBoxSize() / 2.0f;

    Vector2D my_half = hit_box / 2.0f;
    Vector2D obj_half = obj->GetBoxSize() / 2.0f;

    Vector2D diff = my_center - obj_center;

    return (fabsf(diff.x) <= my_half.x + obj_half.x &&
            fabsf(diff.y) <= my_half.y + obj_half.y);
}
