#pragma once
#include "../CharacterBase.h"
#include "../../GameObject.h"
class Player :
    public CharacterBase
{
public:
    Player();
    ~Player();

    //‰Šú‰»ˆ—
    void Initialize(Vector2D _location, Vector2D _box_size) override;
    //XVˆ—
    void Update()override;
    //•`‰æˆ—
    void Draw(Vector2D offset, double rate) override;
    //I—¹ˆ—
    void Finalize()override;

    //“–‚½‚Á‚½‚Ì‹““®
    void OnHitCollision(GameObject* hit_object)override;

    //“ü—Í
    void HandleInput();

};

