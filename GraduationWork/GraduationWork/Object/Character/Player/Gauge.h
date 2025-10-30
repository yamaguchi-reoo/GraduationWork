#pragma once
#include "../../../Utility/Vector2D.h"

// �Q�[�W�̎�ނ�\���񋓌^
enum class GaugeType {
    CircularSection,  // HP�p
    CircularFill      // �e�Q�[�W�p
};

class Gauge 
{
private:
    GaugeType type;         // �Q�[�W�̎��
    int max_value;          // �Q�[�W�̍ő�l
    int current_value;      // ���݂̃Q�[�W�l
    int section_count;      // �Z�N�V�������iHP�Q�[�W�̂Ƃ��̂ݎg�p�j
    unsigned int color;     // �Q�[�W�̐F

	float consumption;      // �e��Ԃ̂Ƃ��̏����
	float recovery;         // ���Ԃ̎��̉񕜗�

    static constexpr int BASE_RADIUS = 60;     // �Q�[�W�̊�{���a
    static constexpr int BASE_THICKNESS = 30;  // �Q�[�W�̊�{����

    float shadow_anim_elapsed;  // �o�ߎ���
    int shadow_frame;
    int shadow_anim[8];
    int circle_base;

    // �_�ŗp�i�񕜒��� 1 �b���Ƃ� ON/OFF�j
    float blink_elapsed;       // �o�ߎ���
    bool  blink_visible;       // ���ݕ\�����邩
    float blink_interval;      // �_�ŊԊu�i�b�j - 1.0f �ɐݒ肷��

    // Gauge.h
    float fade_elapsed;     // �t�F�[�h�p�̎��ԗݐ�
    float fade_speed;       // �t�F�[�h���x�i�������قǂ������j
    int   fade_alpha;       // ���݂̃A���t�@�l�i0?255�j

    //�A�C�e���񕜎��̈ꎞ��~�t���O
        bool is_item_healed = false;
    //�񕜌�̃Q�[�W�l�ێ����ԁi��: 0.1�b�j
        float heal_pause_timer = 0.0f;
    const float HEAL_PAUSE_DURATION = 1.0f;

    // ���݂̉e��Ԃ� Draw ���ŎQ�Ƃ��邽�߂ɕێ�
    bool current_is_shadow;

    int flame_anim[8]{};
    static const int FLAME_W = 174;   // �t���[����
    static const int FLAME_H = 178;   // �t���[����
    static const int OX = FLAME_W / 2; // �s�{�b�gX�i�摜���j
    static const int OY = FLAME_H / 2; // �s�{�b�gY�i�摜���j


public:
    Gauge();

    // �Q�[�W�̏�����
    void Initialize(GaugeType type, int max, int current, int sections, unsigned int color);
    // �ݒ肵�����Q�[�W�l
    void SetValue(int value);

	// �Q�[�W�̍X�V
    void Update(bool is_shadow, float delta);

	// �Q�[�W�̒l���擾
    bool IsEmpty() const { return current_value <= 0; }
    bool IsFull() const { return current_value >= max_value; }

	// �Q�[�W�̕`��
    void Draw(int center_x, int center_y, float scale = 1.0f) const;

    void AddValue(int value);

private:
    // �e�Q�[�W�̉~�`�Q�[�W�`��
    void DrawCircularFill(int center_x, int center_y, float scale) const;

	// HP�Q�[�W�̉~�`�Z�N�V�����`��
    void DrawCircularSection(int center_x, int center_y, float scale) const;

	// �~�ʂ̕`��
    void DrawArc(int cx, int cy, int inner_r, int outer_r, float start_deg, float end_deg, int col) const;

    void UpdateShadowAnimation(float delta);
};

