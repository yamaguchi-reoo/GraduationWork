#include "Effect.h"
#include "../../../../Utility/ResourceManager.h"

void Effect::Initialize()
{
    // ResourceManager のインスタンス取得
    ResourceManager* rm = ResourceManager::GetInstance();

    // エフェクト画像読み込み
    shadow_image = rm->GetImages("Resource/Images/Effect/Retro Impact Effect Pack 5 B.png", 6, 6, 1, 64, 64);
    real_image = rm->GetImages("Resource/Images/Effect/Retro Impact Effect Pack 5 C.png", 8, 8, 1, 64, 64);

    speed = 0.3f; // 再生スピード調整（0.3とか0.5が良い）
    is_active = false;
    frame = 0;
    current_image = nullptr;
}

void Effect::Update()
{
    if (!is_active || !current_image) return;

    frame += speed;
    if (frame >= current_image->size()) {
        is_active = false;
    }
}

void Effect::Draw(Vector2D offset)
{
    if (!is_active || !current_image || current_image->empty()) return;

    int index = static_cast<int>(frame);
    if (index >= current_image->size()) index = current_image->size() - 1;

    int handle = (*current_image)[index];
    if (handle == -1) return;

    Vector2D screen_pos = position - offset;

    DrawRotaGraphF(
        screen_pos.x,
        screen_pos.y,
        1.5, 
        0.0,
        handle,
        TRUE
    );
}

void Effect::Start(Vector2D pos, bool is_shadow)
{
    position = pos;
    frame = 0;
    is_active = true;

    // 影エフェクト or 実体エフェクトを切り替え
    if (is_shadow)
        current_image = &shadow_image;
    else
        current_image = &real_image;
}
