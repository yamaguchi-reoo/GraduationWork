#pragma once
#include "../GameObject.h"
#include "../../common.h"
class Block :
    public GameObject
{
private:

public:
    //初期化処理
    void Initialize(Vector2D _location, Vector2D _box_size) override;
    //更新処理
    void Update()override;
    //描画処理
    void Draw(Vector2D offset, double rate) override;
    //終了時処理
    void Finalize()override;
};
