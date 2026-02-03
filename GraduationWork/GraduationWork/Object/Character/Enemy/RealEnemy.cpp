#include "RealEnemy.h"



void RealEnemy::Initialize(Vector2D _location, Vector2D _box_size)
{
    EnemyBase::Initialize(_location, _box_size);
    // À‘Ìê—p‚Ì‰Šú‰»‚ª‚ ‚ê‚Î‚±‚±‚É’Ç‰Á
    LoadPlayerImage();

    SetAnimation(5, 7, 40);
}

void RealEnemy::Update()
{
    if (situation != prev_state) {
        switch (situation)
        {
        case EnemySituation::Patrol:
            SetAnimation(5, 7, 40);   // •à‚«

            break;

        case EnemySituation::Alert:
            SetAnimation(15, 19, 20);   // ‘Ò‹@

            break;

        case EnemySituation::Charge:
            SetAnimation(5, 7, 20);  // “ËŒ‚

            break;
        }

        prev_state = situation;
    }

    UpdateAnimation();

    EnemyBase::Update();
}



void RealEnemy::Draw(Vector2D offset, double rate)
{
    Vector2D screen_pos = location - offset;
    
    // Šî–{•`‰æ
    EnemyBase::Draw(offset, rate);

    // ‰æ‘œ•`‰æ
    DrawRotaGraphF(
        screen_pos.x + box_size.x / 2,
        screen_pos.y + box_size.y / 2 - 20,
        3.0,
        0.0,
        anim_graph[anim_frame],
        TRUE,
        moving_right ? FALSE : TRUE  // Œü‚«”½“]
    );

    // À‘Ôê—p‚Ì’Ç‰Á•`‰æ
    DrawFormatString(screen_pos.x, screen_pos.y - 20, GetColor(255, 255, 255), "RealEnemy");
    
    #ifdef _DEBUG
    #endif
}

void RealEnemy::LoadPlayerImage()
{
    LoadDivGraph(
        "Resource/Images/Character/Enemy/Shadow dog-Sheet.png", // ‰æ‘œƒpƒX
        anim_total,  // •ªŠ„”
        anim_total,  // ‰¡•ªŠ„
        1,           // c•ªŠ„
        32,          // 1ƒRƒ}•
        32,          // 1ƒRƒ}‚‚³
        anim_graph   // o—Íæ
    );
}


void RealEnemy::UpdateAnimation()
{
    anim_count++;

    if (anim_count >= anim_speed)
    {
        anim_count = 0;
        anim_frame++;

        if (anim_frame > anim_end)
            anim_frame = anim_start;
    }
}

void RealEnemy::SetAnimation(int start, int end, int speed)
{
    anim_start = start;
    anim_end = end;
    anim_frame = start; // æ“ª‚É–ß‚·
    anim_speed = speed;

}


