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



public:
    RealEnemy()
    {
        object_type = REALENEMY;
        // 実体モードに固定
        state = EnemyState::Real;
        max_hp = 5;        // HPちょっと高め
        damage = 2;        // 与えるダメージ強め
        move_speed = 1.5f; // ゆっくり動く

        anim_frame = 0;
        anim_count = 0;
        anim_speed = 5;      // 5フレームごとにコマ送り
        anim_total = 12;     // コマ数（画像に合わせる）
    }

    virtual ~RealEnemy() {}

    void Initialize(Vector2D _location, Vector2D _box_size) override
    {
        EnemyBase::Initialize(_location, _box_size);
        // 実体専用の初期化があればここに追加
    }

    void Update() override
    {
        // 特殊な動きがあれば追加
        // 例：一定間隔で方向転換
        //if (Utility::FrameCounter(120)) // 2秒ごとに反転
        //{
        //    moving_right = !moving_right;
        //}

        EnemyBase::Update();
    }

    void Draw(Vector2D offset, double rate) override;
//    {
//        Vector2D screen_pos = location - offset;
//
//        // 基本描画
//        EnemyBase::Draw(offset, rate);
//
//        // 実態専用の追加描画
//        DrawFormatString(screen_pos.x, screen_pos.y - 20, GetColor(255, 255, 255), "RealEnemy");
//
//#ifdef _DEBUG
//        DrawFormatStringF(location.x + offset.x, location.y + offset.y - 16, GetColor(255, 255, 255), "Enemy HP: %d", hp);
//#endif
//    }


    void OnHitCollision(GameObject* hit_object) override
    {
        EnemyBase::OnHitCollision(hit_object);
        // 実体専用の当たり判定処理を追加できる
    }

    void LoadPlayerImage();

};

