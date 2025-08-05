#pragma once
#include "../GameObject.h"
class Light :
    public GameObject
{
public:
	//‰Šú‰»ˆ—
	virtual void Initialize(Vector2D _location, Vector2D _box_size) override;
	//XVˆ—
	virtual void Update()override;
	//•`‰æˆ—
	virtual void Draw(Vector2D offset, double rate)  override;
	//I—¹ˆ—
	virtual void Finalize()override;
};

