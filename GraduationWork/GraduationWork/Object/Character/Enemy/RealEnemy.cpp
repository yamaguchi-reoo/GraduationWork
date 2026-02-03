#include "RealEnemy.h"

void RealEnemy::Draw(Vector2D offset, double rate)
{
    Vector2D screen_pos = location - offset;
    
    // 基本描画
    EnemyBase::Draw(offset, rate);
    

    // 画像描画
    DrawRotaGraphF(
        screen_pos.x + box_size.x / 2,
        screen_pos.y + box_size.y / 2,
        1.0,
        0.0,
        anim_graph[0],
        TRUE,
        moving_right ? FALSE : TRUE  // 向き反転
    );

    // 実態専用の追加描画
    DrawFormatString(screen_pos.x, screen_pos.y - 20, GetColor(255, 255, 255), "RealEnemy");
    
    #ifdef _DEBUG
            DrawFormatStringF(location.x + offset.x, location.y + offset.y - 16, GetColor(255, 255, 255), "Enemy HP: %d", hp);
    #endif
}

void RealEnemy::LoadPlayerImage()
{
    LoadDivGraph(
        "Resource/Images/Character/Enemy/Shadow dog-Sheet.png", // 画像パス
        anim_total,  // 分割数
        anim_total,  // 横分割
        1,           // 縦分割
        32,          // 1コマ幅
        32,          // 1コマ高さ
        anim_graph   // 出力先
    );
}
