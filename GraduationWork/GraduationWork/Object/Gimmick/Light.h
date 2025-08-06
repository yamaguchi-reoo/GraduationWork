#pragma once
#include "../GameObject.h"
class Light :
    public GameObject
{
private:
	
	float angle;
	float angular_speed = 0.02f;
	float length;
	Vector2D pivot;


public:
	//‰Šú‰»ˆ—
	virtual void Initialize(Vector2D _location, Vector2D _box_size) override;
	//XVˆ—
	virtual void Update()override;
	//•`‰æˆ—
	virtual void Draw(Vector2D offset, double rate)  override;
	//I—¹ˆ—
	virtual void Finalize()override;

	virtual void OnHitCollision(GameObject* hit_object)override;
};

