#pragma once
#include "../CharacterBase.h"
#include "../../GameObject.h"

#include "Gauge.h"
#include <map>

struct AttackHitBox 
{
    Vector2D position = { 0.0f };
    Vector2D size = { 0.0f };
    int frame = 0; 
};

class Player :
    public CharacterBase
{
private:

    enum class PlayerAction { Idle, Walk, Jump, Attack, Death };
    enum class PlayerState { Real, Shadow };
	PlayerAction action; // �v���C���[�̌��݂̍s��
    PlayerState state; // �v���C���[�̏�ԁi���Ԃ��e���j

    //�W�����v
    float jump_velocity;
    const float jump_strength;

    //�U��
    std::vector<AttackHitBox> attack_hitboxes;
    bool is_attacking;
    int attack_cooldown;
    const int attack_cooldown_max;

    Gauge shadow_gauge;     // �e���Q�[�W
    Gauge hp_gauge;         // ���Ԃ�HP�Q�[�W
    int invincible_timer;   // ���G���ԃ^�C�}�[

	//�A�j���[�V����
    std::map<PlayerAction, std::vector<int>> animation_shadow;
	std::map<PlayerAction, std::vector<int>> animation_real;

    std::map<PlayerAction, int> animation_frame_count;
    int animation_frame = 0; //���݂̃A�j���[�V�����t���[��

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
	void UpdateAnimation();

    // �v���C���[�̏�Ԃ̐؂�ւ�
    void SwitchState();

    // �e��Ԃ̍X�V
    void UpdateState();

    // UI�̕`��
    void DrawUI();

    // �v���C���[�̏�Ԃ��擾
    PlayerState GetPlayerState() { return state; }

    bool IsPlayerShadow() const override { return state == PlayerState::Shadow; }

    void LoadPlayerImage();
};

