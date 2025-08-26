#include "Gauge.h"
#include <DxLib.h>
#include <cmath>
#include <algorithm>

#include "../../../Utility/UtilityList.h"

Gauge::Gauge() :
    type(GaugeType::CircularFill), max_value(0), current_value(0),
	section_count(0), color(0), consumption(0.0f), recovery(0.0f)
{
}

void Gauge::Initialize(GaugeType _type, int max, int current, int sections, unsigned int col)
{
    type = _type;
    max_value = max;
    current_value = current;
    section_count = sections;
    color = col;

    consumption = 1.5f; 
    recovery = 1.0f;   

    shadow_anim_elapsed = 0.0f;
    shadow_frame = 0;

    SetTransColor(255, 255, 255); // 白を透過色に設定
    LoadDivGraph("Resource/images/UI/test_shadow2.png", 8, 4, 2, 174, 178, shadow_anim);

    circle_base = LoadGraph("Resource/images/UI/circle_base.png"); // 1枚

    int sheet = LoadGraph("Resource/images/UI/flames_only_sheet.png"); // 4x2

    // 4x2に分割（174x178）
    int idx = 0;
    for (int r = 0; r < 2; ++r) {
        for (int c = 0; c < 4; ++c) {
            flame_anim[idx++] = DerivationGraph(c * FLAME_W, r * FLAME_H, FLAME_W, FLAME_H, sheet);
        }
    }




}

void Gauge::SetValue(int value)
{
	// 範囲を0からmax_valueの間に制限
    current_value = Clamp(value, 0, max_value);
}

void Gauge::Update(bool is_shadow, float delta)
{
	// 影状態のときは消費、実態のときは回復
    if(is_shadow) 
    {
        current_value -= static_cast<int>(consumption);        
    }
    else 
    {
        current_value += static_cast<int>(recovery);
    }

	//範囲を0からmax_valueの間に制限
    current_value = Clamp(current_value, 0, max_value);

    // ★ 影状態のときだけ Shadow アニメを進める。影でなければリセットして非表示に。
    if (is_shadow) {
        UpdateShadowAnimation(delta);
    }
    else {
        shadow_anim_elapsed = 0.0f;
        shadow_frame = 0; // 先頭に戻す（任意）
    }

    //UpdateShadowAnimation(delta);
}

void Gauge::Draw(int center_x, int center_y, float scale) const 
{
    switch (type) 
    {
    case GaugeType::CircularFill:
        DrawCircularFill(center_x, center_y, scale);
        DrawFormatString(1100, 100, GetColor(255, 255, 255), "最大ゲージ量: %d", max_value);
        DrawFormatString(1100, 120, GetColor(255, 255, 255), "現在のゲージ量: %d", current_value);
        break;
    case GaugeType::CircularSection:
        DrawCircularSection(center_x, center_y, scale);
        break;
    }

  
}

void Gauge::DrawCircularFill(int cx, int cy, float scale) const
{
    int outer = static_cast<int>(BASE_RADIUS * scale);
    int inner = outer - static_cast<int>(BASE_THICKNESS * scale);

    float rate = Clamp(static_cast<float>(current_value) / max_value, 0.0f, 1.0f);
    float fill_angle = 360.0f * rate;

    // ---- ゲージ本体（円・数値） ----
    DrawArc(cx, cy, inner, outer, 0, 360, GetColor(40, 40, 40));        // 背景
    DrawArc(cx, cy, inner, outer, -150, -150 + fill_angle, color);      // 本体
    DrawCircleAA(cx, cy, inner - 1, 64, GetColor(0, 0, 0), TRUE);        // 中央空洞

    // ---- 円の縁（静止パーツ）を必ず同じ位置に描く ----
    if (circle_base != -1) {
        // 画像内の中心(OX,OY) = 画面の(cx,cy) に一致させる
        DrawRotaGraph2(cx, cy, OX, OY, scale, 0.0, circle_base, TRUE);
    }

    // ---- 炎（アニメ）だけ重ねる ----
    if (flame_anim[shadow_frame] != -1) {
        SetDrawBlendMode(DX_BLENDMODE_ADD, 180); // お好みで
        DrawRotaGraph2(cx, cy, OX, OY, scale, 0.0, flame_anim[shadow_frame], TRUE);
        SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
    }
}

void Gauge::DrawCircularSection(int cx, int cy, float scale) const
{
	// 円形セクションゲージの描画
    int outer = static_cast<int>(BASE_RADIUS * scale);
    int inner = outer - static_cast<int>(BASE_THICKNESS * scale);
    float angle_per = 360.0f / section_count;

    DrawArc(cx, cy, inner, outer, 0, 360, GetColor(80, 80, 80)); // 背景

       // ---- 円の縁（静止パーツ）を必ず同じ位置に描く ----
    if (circle_base != -1) {
        // 画像内の中心(OX,OY) = 画面の(cx,cy) に一致させる
        DrawRotaGraph2(cx, cy, OX, OY, scale, 0.0, circle_base, TRUE);
    }

    for (int i = 0; i < section_count; ++i) {
        float angle = i * angle_per;
        DrawArc(cx, cy, inner, outer, angle, angle + 0.5f, GetColor(120, 120, 120)); // 区切り線
    }

    for (int i = 0; i < current_value; ++i) {
        float start = i * angle_per;
        DrawArc(cx, cy, inner, outer, start - 150, start - 150 + angle_per, color); // 実際のセクション
    }
}

void Gauge::DrawArc(int cx, int cy, int r1, int r2, float deg_start, float deg_end, int col) const 
{
	// 円弧の描画
    for (float angle = deg_start; angle < deg_end; angle += 2.0f) {
        float rad = angle * DX_PI_F / 180.0f;
        int x1 = cx + cosf(rad) * r1;
        int y1 = cy + sinf(rad) * r1;
        int x2 = cx + cosf(rad) * r2;
        int y2 = cy + sinf(rad) * r2;
        DrawLine(x1, y1, x2, y2, col);
    }
}

void Gauge::UpdateShadowAnimation(float delta)
{
    shadow_anim_elapsed += delta;

    const float frame_interval = 1.0f / 12.0f; // 12fpsの火アニメ

    if (shadow_anim_elapsed >= frame_interval)
    {
        shadow_frame = (shadow_frame + 1) % 8;
        shadow_anim_elapsed -= frame_interval;
    }
}
