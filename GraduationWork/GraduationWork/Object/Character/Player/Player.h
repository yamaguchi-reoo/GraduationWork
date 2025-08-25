#pragma once
#include "../CharacterBase.h"
#include "../../GameObject.h"

#include "Gauge.h"

class Player :
    public CharacterBase
{
private:

    enum class PlayerState { Real, Shadow };
    PlayerState state; // �v���C���[�̏�ԁi���Ԃ��e���j

    //�W�����v
    float jump_velocity;
    const float jump_strength; 

    //�U��
    struct AttackHitBox { Vector2D position; Vector2D size; int frame; };
    std::vector<AttackHitBox> attack_hitboxes;
    bool is_attacking;
    int attack_cooldown;
    const int attack_cooldown_max;

	Gauge shadow_gauge; // �e���Q�[�W
    Gauge hp_gauge; // ���Ԃ�HP�Q�[�W
    int invincible_timer; // ���G���ԃ^�C�}�[

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

public:
    //����
    void HandleInput();

    void UpdateJump();
    void UpdateAttack();

	// �v���C���[�̏�Ԃ̐؂�ւ�
    void SwitchState();

	// �e��Ԃ̍X�V
	void UpdateState();

	// UI�̕`��
	void DrawUI();

	// �v���C���[�̏�Ԃ��擾
    PlayerState GetPlayerState(){return state;}

    bool IsPlayerShadow() const override { return state == PlayerState::Shadow; }
};

