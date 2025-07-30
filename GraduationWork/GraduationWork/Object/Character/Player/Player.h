#pragma once
#include "../CharacterBase.h"
#include "../../GameObject.h"

enum class PlayerState {
    Real,   // ����
    Shadow  // �e���
};

class Player :
    public CharacterBase
{
private:
	PlayerState state; // �v���C���[�̏�ԁi���Ԃ��e���j

	float shadow_gauge; //�e���Q�[�W
	float shadow_gauge_max; //�e���Q�[�W�̍ő�l
	float shadow_consumption; //�e���Q�[�W�̏����


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

	// �v���C���[�̏�Ԃ̐؂�ւ�
    void SwitchState();

	// �e��Ԃ̍X�V
	void UpdateState();

	// UI�̕`��
	void DrawUI();

	// �e���Q�[�W�̕`��
    void DrawCircularShadowGauge(int center_x, int center_y, float scale);

	// ���Ԃ�HP�Q�[�W��`��
    void DrawRealHPGauge(int center_x, int center_y, float scale);

	// �A�[�N�̕`��
    void DrawArc(int center_x, int center_y, int inner_radius, int outer_radius, float start_angle, float end_angle, int color);
};

