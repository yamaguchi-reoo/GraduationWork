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
	Vector2D screen_pos = location + offset;
	if (screen_pos.x + box_size.x >= 0 && screen_pos.x < SCREEN_WIDTH)
	{
		//if (image == NULL) return; // 画像が設定されていない場合は描画しない
		// 画像を描画（中心座標指定）
		DrawRotaGraphF(offset.x + (box_size.x / 2), offset.y + (box_size.y / 2), rate, 0.0, image, TRUE, flip_flg);

	}

#ifdef _DEBUG
	// ヒットボックスの中心座標
	Vector2D center = location + hit_box / 2.0f;

	// 左上と右下座標を計算（オフセットは加えない）
	Vector2D top_left = center - hit_box / 2.0f;
	Vector2D bottom_right = center + hit_box / 2.0f;

	// 枠線だけ赤色で描画
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
	//自分の左上座標
	Vector2D my_pos = location;
	//自分の幅と高さの半分
	Vector2D my_size = hit_box / 2.0f;

	//相手の左上座標
	Vector2D sub_pos = obj->GetLocation();
	//相手の幅と高さの半分
	Vector2D sub_size = obj->GetBoxSize() / 2.0f;

	//中心座標の差分
	Vector2D diff = (my_pos + my_size) - (sub_pos + sub_size);

	//当たり判定の演算
	return (fabsf(diff.x) <= my_size.x + sub_size.x &&
		fabsf(diff.y) <= my_size.y + sub_size.y);

}
