#pragma once
#include <DxLib.h>
#include "EnemyBase.h"

class RealEnemy :
    public EnemyBase
{
protected:
    int anim_graph[25];     // 分割した画像を格納
    int anim_frame;         // 現在のコマ番号
    int anim_count;         // コマ送り用カウンタ
    int anim_speed;         // 何フレームで1コマ進めるか
    int anim_total;         // コマ数

    // アニメ範囲
    int anim_start;
    int anim_end;

    EnemySituation prev_state;

public:
    RealEnemy()
    {
        object_type = REALENEMY;
        // 実体モードに固定
        state = EnemyState::Real;
        max_hp = 5;        // HPちょっと高め
        damage = 2;        // 与えるダメージ強め
        move_speed = 1.5f; // ゆっくり動く

        for (int i = 0; i < 25; ++i)
            anim_graph[i] = -1;

        anim_frame = 0;
        anim_count = 0;
        anim_speed = 20;      // 5フレームごとにコマ送り
        anim_total = 25;     // コマ数（画像に合わせる

        anim_start = 0;
        anim_end = 0;

        prev_state = EnemySituation::Patrol;

    }

    virtual ~RealEnemy() {}

    void Initialize(Vector2D _location, Vector2D _box_size) override;
    

    void Update() override;
    

    void Draw(Vector2D offset, double rate) override;


    void OnHitCollision(GameObject* hit_object) override
    {
        EnemyBase::OnHitCollision(hit_object);
        // 実体専用の当たり判定処理を追加できる
    }

    void LoadPlayerImage();

    void UpdateAnimation();

    void SetAnimation(int start, int end, int speed);
};

