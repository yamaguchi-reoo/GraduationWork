#pragma once
#include "../../../Utility/Vector2D.h"

// ゲージの種類を表す列挙型
enum class GaugeType {
    CircularSection,  // HP用
    CircularFill      // 影ゲージ用
};

class Gauge 
{
private:
    GaugeType type;         // ゲージの種類
    int max_value;          // ゲージの最大値
    int current_value;      // 現在のゲージ値
    int section_count;      // セクション数（HPゲージのときのみ使用）
    unsigned int color;     // ゲージの色

	float consumption;      // 影状態のときの消費量
	float recovery;         // 実態の時の回復量

    static constexpr int BASE_RADIUS = 60;     // ゲージの基本半径
    static constexpr int BASE_THICKNESS = 30;  // ゲージの基本厚み

    float shadow_anim_elapsed;  // 経過時間
    int shadow_frame;
    int shadow_anim[8];
    int circle_base;

    // 点滅用（回復中に 1 秒ごとに ON/OFF）
    float blink_elapsed;       // 経過時間
    bool  blink_visible;       // 現在表示するか
    float blink_interval;      // 点滅間隔（秒） - 1.0f に設定する

    // Gauge.h
    float fade_elapsed;     // フェード用の時間累積
    float fade_speed;       // フェード速度（小さいほどゆっくり）
    int   fade_alpha;       // 現在のアルファ値（0?255）

    //アイテム回復時の一時停止フラグ
        bool is_item_healed = false;
    //回復後のゲージ値保持時間（例: 0.1秒）
        float heal_pause_timer = 0.0f;
    const float HEAL_PAUSE_DURATION = 1.0f;

    // 現在の影状態を Draw 側で参照するために保持
    bool current_is_shadow;

    int flame_anim[8]{};
    static const int FLAME_W = 174;   // フレーム幅
    static const int FLAME_H = 178;   // フレーム高
    static const int OX = FLAME_W / 2; // ピボットX（画像内）
    static const int OY = FLAME_H / 2; // ピボットY（画像内）


public:
    Gauge();

    // ゲージの初期化
    void Initialize(GaugeType type, int max, int current, int sections, unsigned int color);
    // 設定したいゲージ値
    void SetValue(int value);

	// ゲージの更新
    void Update(bool is_shadow, float delta);

	// ゲージの値を取得
    bool IsEmpty() const { return current_value <= 0; }
    bool IsFull() const { return current_value >= max_value; }

	// ゲージの描画
    void Draw(int center_x, int center_y, float scale = 1.0f) const;

    void AddValue(int value);

private:
    // 影ゲージの円形ゲージ描画
    void DrawCircularFill(int center_x, int center_y, float scale) const;

	// HPゲージの円形セクション描画
    void DrawCircularSection(int center_x, int center_y, float scale) const;

	// 円弧の描画
    void DrawArc(int cx, int cy, int inner_r, int outer_r, float start_deg, float end_deg, int col) const;

    void UpdateShadowAnimation(float delta);
};

