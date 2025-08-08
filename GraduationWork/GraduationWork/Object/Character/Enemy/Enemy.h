#pragma once
#include "../CharacterBase.h"
#include "../../GameObject.h"

class Enemy :
    public CharacterBase
{
private:
    int max_hp;
    int damage;
    bool is_dead;
    bool is_alive;

public:
    Enemy();
    ~Enemy();

    //����������
    void Initialize(Vector2D _location, Vector2D _box_size) override;
    //�X�V����
    void Update()override;
    //�`�揈��
    void Draw(Vector2D offset, double rate) override;
    //�I��������
    void Finalize()override;


    // �Փˏ���
    void OnHitCollision(GameObject* hit_object) override;

    // �_���[�W����
    void ReceiveDamage(int dmg);

    // ���S�`�F�b�N
    bool IsDead() const { return is_dead; }

};