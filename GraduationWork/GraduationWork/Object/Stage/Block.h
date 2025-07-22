#pragma once
#include "../GameObject.h"
#include "../../common.h"
class Block :
    public GameObject
{
private:

public:
    //‰Šú‰»ˆ—
    void Initialize(Vector2D _location, Vector2D _box_size) override;
    //XVˆ—
    void Update()override;
    //•`‰æˆ—
    void Draw(Vector2D offset, double rate) override;
    //I—¹ˆ—
    void Finalize()override;
};
