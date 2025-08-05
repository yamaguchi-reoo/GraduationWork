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

public:
    Gauge();

    // ゲージの初期化
    void Initialize(GaugeType type, int max, int current, int sections, unsigned int color);
    // 設定したいゲージ値
    void SetValue(int value);

	// ゲージの更新
    void Update(bool is_shadow);

	// ゲージの値を取得
    bool IsEmpty() const { return current_value <= 0; }
    bool IsFull() const { return current_value >= max_value; }

	// ゲージの描画
    void Draw(int center_x, int center_y, float scale = 1.0f) const;

private:
    // 影ゲージの円形ゲージ描画
    void DrawCircularFill(int center_x, int center_y, float scale) const;

	// HPゲージの円形セクション描画
    void DrawCircularSection(int center_x, int center_y, float scale) const;

	// 円弧の描画
    void DrawArc(int cx, int cy, int inner_r, int outer_r, float start_deg, float end_deg, int col) const;
};

