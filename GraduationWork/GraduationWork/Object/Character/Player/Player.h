#pragma once
#include "../CharacterBase.h"
#include "../../GameObject.h"
class Player :
    public CharacterBase
{
public:
    Player();
    ~Player();

    //����������
    void Initialize(Vector2D _location, Vector2D _box_size) override;
    //�X�V����
    void Update()override;
    //�`�揈��
    void Draw(Vector2D offset, double rate) override;
    //�I��������
    void Finalize()override;

    //�����������̋���
    void OnHitCollision(GameObject* hit_object)override;

    //����
    void HandleInput();

};

