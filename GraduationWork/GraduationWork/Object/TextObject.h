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
        position = pos;
        size = { 48.0f, 48.0f };
        message = msg;
    }

    void Update() override
    {
        // プレイヤーとの距離で表示切り替え
        auto player = ObjectManager::FindObject<Player>();
        if (player)
        {
            float dx = player->position.x - position.x;
            float dy = player->position.y - position.y;
            float dist = sqrtf(dx * dx + dy * dy);
            is_visible = (dist < 100.0f); // 100px以内で表示
        }
    }

    void Draw(Vector2D offset = {}, double rate = 1.0) override
    {
        if (!is_visible) return;

        int draw_x = static_cast<int>(position.x - offset.x);
        int draw_y = static_cast<int>(position.y - offset.y);

        SetDrawBlendMode(DX_BLENDMODE_ALPHA, 180);
        DrawBox(draw_x - 4, draw_y - 24, draw_x + 100, draw_y - 4, GetColor(0, 0, 0), TRUE);
        SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
        DrawFormatString(draw_x, draw_y - 22, GetColor(255, 255, 255), message.c_str());
    }

};
