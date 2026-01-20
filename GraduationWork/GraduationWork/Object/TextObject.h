#pragma once
#include "../Object/GameObject.h"
#include "Character/Player/Player.h"        // ← これを追加（FindObjectでPlayerを探すため）
#include "../Scene/GamaScene/InGame/InGameScene.h"       // ← Cameraを使うなら必要
#include "ObjectManager.h"        // ← FindObject用

class TextObject : public GameObject
{
public:
    std::string message;
    bool is_visible = false;

    TextObject(const Vector2D& pos, const std::string& msg)
    {
        object_type = eObjectType::TEXT;   // ★必ず必要
        position = pos;
        size = { 48.0f, 48.0f };
        message = msg;
    }
public:
    TextObject(){};

    void Update() override
    {
        // プレイヤーとの距離で表示切り替え
        auto player = object_manager->FindObjectType(PLAYER);
        if (player)
        {
            float dx = player->GetLocation().x - GetLocation().x;
            float dy = player->GetLocation().y - GetLocation().y;
            float dist = sqrtf(dx * dx + dy * dy);
            is_visible = (dist < 100.0f); // 100px以内で表示
        }
    }


    void Draw(Vector2D offset, double rate) override
    {
        if (!is_visible) return;

        int draw_x = static_cast<int>(position.x - offset.x);
        int draw_y = static_cast<int>(position.y - offset.y);

        // 背景（黒）
        SetDrawBlendMode(DX_BLENDMODE_ALPHA, 150);
        DrawBox(draw_x - 10, draw_y - 30, draw_x + 200, draw_y - 10, GetColor(0, 0, 0), TRUE);
        SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

        // 白文字
        DrawFormatString(draw_x - 4, draw_y - 28, GetColor(255, 255, 255), "%s", message.c_str());
    }



};
