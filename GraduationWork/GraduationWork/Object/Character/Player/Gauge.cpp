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
}

void Gauge::SetValue(int value)
{
	// ”ÍˆÍ‚ğ0‚©‚çmax_value‚ÌŠÔ‚É§ŒÀ
    current_value = Clamp(value, 0, max_value);
}

void Gauge::Update(bool is_shadow)
{
	// ‰eó‘Ô‚Ì‚Æ‚«‚ÍÁ”ïAÀ‘Ô‚Ì‚Æ‚«‚Í‰ñ•œ
    if(is_shadow) 
    {
        current_value -= static_cast<int>(consumption);        
    }
    else 
    {
        current_value += static_cast<int>(recovery);
    }

	//”ÍˆÍ‚ğ0‚©‚çmax_value‚ÌŠÔ‚É§ŒÀ
    current_value = Clamp(current_value, 0, max_value);
}

void Gauge::Draw(int center_x, int center_y, float scale) const 
{
    switch (type) 
    {
    case GaugeType::CircularFill:
        DrawCircularFill(center_x, center_y, scale);
        DrawFormatString(1100, 100, GetColor(255, 255, 255), "Å‘åƒQ[ƒW—Ê: %d", max_value);
        DrawFormatString(1100, 120, GetColor(255, 255, 255), "Œ»İ‚ÌƒQ[ƒW—Ê: %d", current_value);
        break;
    case GaugeType::CircularSection:
        DrawCircularSection(center_x, center_y, scale);
        break;
    }

  
}

void Gauge::DrawCircularFill(int cx, int cy, float scale) const
{
	// ‰~Œ`ƒQ[ƒW‚Ì•`‰æ
    int outer = static_cast<int>(BASE_RADIUS * scale);
    int inner = outer - static_cast<int>(BASE_THICKNESS * scale);


    float rate = Clamp(static_cast<float>(current_value) / max_value, 0.0f, 1.0f);
    float fill_angle = 360.0f * rate;


    DrawArc(cx, cy, inner, outer, 0, 360, GetColor(40, 40, 40)); // ”wŒi
    DrawArc(cx, cy, inner, outer, 0, fill_angle, color);         // –{‘Ì
    DrawCircleAA(cx, cy, inner - 1, 64, GetColor(0, 0, 0), TRUE); // ’†‰›‚ğ‹ó“´‚É
}

void Gauge::DrawCircularSection(int cx, int cy, float scale) const
{
	// ‰~Œ`ƒZƒNƒVƒ‡ƒ“ƒQ[ƒW‚Ì•`‰æ
    int outer = static_cast<int>(BASE_RADIUS * scale);
    int inner = outer - static_cast<int>(BASE_THICKNESS * scale);
    float angle_per = 360.0f / section_count;

    DrawArc(cx, cy, inner, outer, 0, 360, GetColor(80, 80, 80)); // ”wŒi

    for (int i = 0; i < section_count; ++i) {
        float angle = i * angle_per;
        DrawArc(cx, cy, inner, outer, angle, angle + 0.5f, GetColor(120, 120, 120)); // ‹æØ‚èü
    }

    for (int i = 0; i < current_value; ++i) {
        float start = i * angle_per;
        DrawArc(cx, cy, inner, outer, start, start + angle_per, color); // ÀÛ‚ÌƒZƒNƒVƒ‡ƒ“
    }
}

void Gauge::DrawArc(int cx, int cy, int r1, int r2, float deg_start, float deg_end, int col) const 
{
	// ‰~ŒÊ‚Ì•`‰æ
    for (float angle = deg_start; angle < deg_end; angle += 2.0f) {
        float rad = angle * DX_PI_F / 180.0f;
        int x1 = cx + cosf(rad) * r1;
        int y1 = cy + sinf(rad) * r1;
        int x2 = cx + cosf(rad) * r2;
        int y2 = cy + sinf(rad) * r2;
        DrawLine(x1, y1, x2, y2, col);
    }
}
