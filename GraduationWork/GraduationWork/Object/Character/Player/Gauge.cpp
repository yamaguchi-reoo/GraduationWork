#include "Gauge.h"
#include <DxLib.h>
#include <cmath>
#include <algorithm>

#include "../../../Utility/UtilityList.h"
#include "../../../Scene/SceneManager.h"

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

    circle_base = LoadGraph("Resource/images/UI/Darksiders Concept.png"); // 1枚

    int sheet = LoadGraph("Resource/images/UI/flames_only_sheet.png"); // 4x2

    // 4x2に分割（174x178）
    int idx = 0;
    for (int r = 0; r < 2; ++r) {
        for (int c = 0; c < 4; ++c) {
            flame_anim[idx++] = DerivationGraph(c * FLAME_W, r * FLAME_H, FLAME_W, FLAME_H, sheet);
        }
    }

    // 点滅の初期化
    blink_elapsed = 5.0f;
    blink_visible = true;
    blink_interval = 5.0f; // 5秒間隔
    current_is_shadow = false;

    fade_elapsed = 0.0f;
    fade_speed = 1.0f; 
    fade_alpha = 255;
    switchSpeed = 30.0f; // ← 回転速度（1フレームあたりの角度）



}

void Gauge::SetValue(int value)
{
	// 範囲を0からmax_valueの間に制限
    current_value = Clamp(value, 0, max_value);
}

void Gauge::Update(bool is_shadow, float delta)
{
    // 現在状態を保存（Draw が参照するため）
    current_is_shadow = is_shadow;

    // 影状態のときは消費、実態のときは回復
    if (is_shadow)
    {
        current_value -= static_cast<int>(consumption);
    }
    else
    {
        current_value += static_cast<int>(recovery);
    }


    // 範囲を0からmax_valueの間に制限
    current_value = Clamp(current_value, 0, max_value);

    if (is_shadow) {
        // 影状態のときだけ Shadow アニメを進める。影でなければリセットして非表示に。
        UpdateShadowAnimation(delta);

        // 回復用の点滅は影でないときに使うのでリセットしておく
        blink_elapsed = 0.0f;
        blink_visible = true;
    }
    else {
        // 回復中 → フェード風点滅
        fade_elapsed += delta * fade_speed;

        // サイン波で 0〜255 を往復させる
        float t = (sinf(fade_elapsed * DX_PI_F) * 0.5f + 0.5f); // 0.0〜1.0
        fade_alpha = static_cast<int>(t * 255);

        // 影アニメは停止・先頭に戻す
        shadow_anim_elapsed = 0.0f;
        shadow_frame = 0;
    }

    // --- ゲージ残量が少ないときに点滅（300以下） ---
    if (current_is_shadow && current_value <= 300)
    {
        blink_elapsed += delta;

        // 点滅速度（1秒周期でON/OFF切替）
        const float blink_interval = 1.0f;

        if (blink_elapsed >= blink_interval)
        {
            blink_elapsed = 0.0f;
            blink_visible = !blink_visible; // 表示・非表示を切替
        }
    }
    else
    {
        blink_visible = true; // 通常は常に表示
    }


    //ゲージ回転して切り替え
    if (isSwitching)
    {
        switchAngle += switchSpeed;
        if (switchAngle >= 180.0f)
        {
            switchAngle = 0.0f;
            isSwitching = false;
        }
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
    // 回復中で点滅OFFなら、枠（circle_base）だけ描いて早期リターン
    if (!current_is_shadow && !blink_visible) {
        if (circle_base != -1) {
            DrawRotaGraph2(cx, cy, OX, OY, scale, 0.0, circle_base, TRUE);
        }
        return;
    }

    if (isSwitching)
    {
        float rad = switchAngle * DX_PI_F / 180.0f;

        // 回転演出（基底画像を回転）
        SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
        DrawRotaGraph(cx, cy, scale, rad, circle_base, TRUE);
        SetDrawBlendMode(DX_BLENDMODE_ALPHA, (int)(255 - (switchAngle / 180.0f) * 255));
        DrawRotaGraph(cx, cy, scale, rad + DX_PI_F, circle_base, TRUE);
        SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
        return; // 通常描画はスキップ
    }


    int outer = static_cast<int>(BASE_RADIUS * scale);
    int inner = outer - static_cast<int>(BASE_THICKNESS * scale);

    float rate = Clamp(static_cast<float>(current_value) / max_value, 0.0f, 1.0f);
    float fill_angle = 360.0f * rate;

    // ---- ゲージ本体（円・数値） ----
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, fade_alpha);  // ← フェードを反映
    DrawArc(cx, cy, inner, outer, 0, 360, GetColor(40, 40, 40));
    DrawArc(cx, cy, inner, outer, -90 - fill_angle, - 90, color);
    DrawCircleAA(cx, cy, inner - 1, 64, GetColor(0, 0, 0), TRUE);
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);         // ← リセット

    // ---- 円の縁（静止パーツ）を必ず同じ位置に描く ----
    if (circle_base != -1) {
        DrawRotaGraph2(cx, cy, OX, OY, scale, 0.0, circle_base, TRUE);
    }

    // ---- 炎（アニメ）は従来通り影用として描画（必要ならこちらも条件付け可） ----
    if (flame_anim[shadow_frame] != -1) {
        SetDrawBlendMode(DX_BLENDMODE_ADD, 180);
        DrawRotaGraph2(cx, cy, OX, OY, scale, 0.0, flame_anim[shadow_frame], TRUE);
        SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
    }

    // デバッグ表示（数値）も点滅対象にしたいのでここで描画
    DrawFormatString(1100, 100, GetColor(255, 255, 255), "最大ゲージ量: %d", max_value);
    DrawFormatString(1100, 120, GetColor(255, 255, 255), "現在のゲージ量: %d", current_value);
}

void Gauge::DrawCircularSection(int cx, int cy, float scale) const
{
    // 回復中で点滅OFFなら、枠（circle_base）だけ描いて早期リターン
    if (!current_is_shadow && !blink_visible) {
        if (circle_base != -1) {
            DrawRotaGraph2(cx, cy, OX, OY, scale, 0.0, circle_base, TRUE);
        }
        return;
    }

    // 以下は既存の処理
    int outer = static_cast<int>(BASE_RADIUS * scale);
    int inner = outer - static_cast<int>(BASE_THICKNESS * scale);
    float angle_per = 360.0f / section_count;

    DrawArc(cx, cy, inner, outer, 0, 360, GetColor(80, 80, 80)); // 背景

    if (circle_base != -1) {
        DrawRotaGraph2(cx, cy, OX, OY, scale, 0.0, circle_base, TRUE);
    }

    for (int i = 0; i < section_count; ++i) {
        float angle = i * angle_per;
        DrawArc(cx, cy, inner, outer, angle, angle + 0.5f, GetColor(120, 120, 120)); // 区切り線
    }

    for (int i = 0; i < current_value; ++i) {
        float start = -i * angle_per;
        DrawArc(cx, cy, inner, outer, start+150 , start+150  + angle_per, color); // 実際のセクション
    }

    //DrawStringToHandle(100, 100, "HELLO,WORLD", GetColor(255, 255, 255), SceneManager::font);
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

void Gauge::AddValue(int value)
{
    // 現在値に回復量を加算した新しい値を計算
    int new_value = current_value + value;

    // 既存の安全なSetValue()メソッドを使って値を更新
    // SetValue()にはClamp(範囲制限)の処理が含まれているため安全。
    SetValue(new_value);

    //回復が行われたら一時停止フラグを立てる
        is_item_healed = true;
    heal_pause_timer = HEAL_PAUSE_DURATION;
}

void Gauge::StartSwitch(bool toShadow)
{
    if (isSwitching) return;
    isSwitching = true;
    switchAngle = 0.0f;
    switchToShadow = toShadow;
}

bool Gauge::IsSwitching() const
{
    return isSwitching;
}