#pragma once
#include "../Utility/Vector2D.h"
#include "ObjectType.h"

class GameObject
{
protected:
	Vector2D location;	//オブジェクトの位置
	Vector2D box_size;	//オブジェクトのサイズ
	Vector2D hit_box;	//当たり判定のサイズ

	Vector2D velocity = { 0.0f };
	int image;			//描画する画像データ
	int flip_flg;		//描画する反転フラグ

	eObjectType object_type;		//オブジェクトの種類

	int hp = 0;
	bool is_delete = false;
public:
	GameObject();
	~GameObject();

	//初期化処理
	virtual void Initialize(Vector2D _location, Vector2D _box_size);
	//更新処理
	virtual void Update();
	//描画処理
	virtual void Draw(Vector2D offset, double rate);
	//終了時処理
	virtual void Finalize();

public:
	//座標を設定
	void SetLocation(Vector2D _location);

	//座標を取得
	Vector2D GetLocation()const;

	//ボックスサイズを取得
	Vector2D GetBoxSize()const;

	//速度取得
	Vector2D GetVelocity()const;

	//オブジェクトタイプ取得	
	eObjectType GetObjectType();

	virtual void OnHitCollision(GameObject* hit_object);

	//当たり判定
	bool CheckBoxCollision(GameObject* obj);

	//削除フラグを取得
	bool GetIsDelete() { return is_delete; }

	void SetDelete() { is_delete = true; }

};

