#pragma once
#include <DxLib.h>
#include "EnemyBase.h"

class RealEnemy :
    public EnemyBase
{
public:
    RealEnemy()
    {
        // ���̃��[�h�ɌŒ�
        state = EnemyState::Real;
        max_hp = 5;        // HP������ƍ���
        damage = 2;        // �^����_���[�W����
        move_speed = 1.5f; // ������蓮��
    }

    virtual ~RealEnemy() {}

    void Initialize(Vector2D _location, Vector2D _box_size) override
    {
        EnemyBase::Initialize(_location, _box_size);
        // ���̐�p�̏�����������΂����ɒǉ�
    }

    void Update() override
    {
        // ����ȓ���������Βǉ�
        // ��F���Ԋu�ŕ����]��
        //if (Utility::FrameCounter(120)) // 2�b���Ƃɔ��]
        //{
        //    moving_right = !moving_right;
        //}

        EnemyBase::Update();
    }

    void Draw(Vector2D offset, double rate) override
    {
        Vector2D screen_pos = location - offset;

        // ��{�`��
        EnemyBase::Draw(offset, rate);

        // ���Ԑ�p�̒ǉ��`��
        DrawFormatString(screen_pos.x, screen_pos.y - 20, GetColor(255, 255, 255), "RealEnemy");
    }


    void OnHitCollision(GameObject* hit_object) override
    {
        EnemyBase::OnHitCollision(hit_object);
        // ���̐�p�̓����蔻�菈����ǉ��ł���
    }

};

