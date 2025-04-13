#pragma once

#include "../../../../Utils/Vector/Vector.h"

// ------------------------------------------------------------
// Color 변환 관련 매크로 및 유틸 함수
// ------------------------------------------------------------
#define COL32_R_SHIFT    0
#define COL32_G_SHIFT    8
#define COL32_B_SHIFT    16
#define COL32_A_SHIFT    24
#define COL32_A_MASK     0xFF000000

static inline float ColSaturate(float f) { return (f < 0.0f) ? 0.0f : (f > 1.0f) ? 1.0f : f; }
#define F32_TO_INT8_SAT(_VAL) ((int)(ColSaturate(_VAL) * 255.0f + 0.5f))

// ------------------------------------------------------------
// Color 클래스 (RGBA float 기반 색 표현 및 변환 지원)
// ------------------------------------------------------------
class Color
{
    float x, y, z, w; // r, g, b, a

public:
    Color() {}
    Color(BYTE r, BYTE g, BYTE b, BYTE a = 255)
    {
        float sc = 1.0f / 255.0f;
        x = r * sc; y = g * sc; z = b * sc; w = a * sc;
    }

    Color(uint32_t rgba)
    {
        float sc = 1.0f / 255.0f;
        x = ((rgba >> COL32_R_SHIFT) & 0xFF) * sc;
        y = ((rgba >> COL32_G_SHIFT) & 0xFF) * sc;
        z = ((rgba >> COL32_B_SHIFT) & 0xFF) * sc;
        w = ((rgba >> COL32_A_SHIFT) & 0xFF) * sc;
    }

    operator uint32_t() const
    {
        uint32_t out = 0;
        out |= F32_TO_INT8_SAT(x) << COL32_R_SHIFT;
        out |= F32_TO_INT8_SAT(y) << COL32_G_SHIFT;
        out |= F32_TO_INT8_SAT(z) << COL32_B_SHIFT;
        out |= F32_TO_INT8_SAT(w) << COL32_A_SHIFT;
        return out;
    }
};

// ------------------------------------------------------------
// Colors: 색상 팔레트 정의
// ------------------------------------------------------------
namespace Colors {
namespace Simple {

    // 일반 색상
    const Color stasis        {112, 128, 147, 255};
    const Color dark_red      {255,   0,   0, 150};
    const Color rudy          {255,   0,  40, 255};
    const Color lime          {200, 236,  70, 255};
    const Color drop_shadow   {  0,   0,   0, 150};
    const Color pacific_blue  { 13, 165, 206, 255};
    const Color exp_bar       {140,  90, 226, 255};
    const Color exp_bar_bg    { 20,  20,  20, 160};

    // 레거시 테마
    namespace Legacy {
        const Color background   {  0,   0,   0, 180 };
        const Color foreground   {120, 225, 120, 230 };
        const Color White_font   {255, 255, 255, 255 };
        const Color black        {  0,   0,   0, 255 };
        const Color orange       {230, 145,  30, 255 };
        const Color red          {255,  27,  18, 255 };
        const Color border       { 68,  68,  68, 255 };
    }

    // 모던 테마
    namespace Modern {
        const Color border       { 68,  68,  68, 255 };
        const Color background   { 34,  31,  27, 255 };
        const Color green        {124, 227,   9, 255 };
        const Color red          {255,  27,  18, 255 };
    }

}} // namespace Colors

// ------------------------------------------------------------
// BackGroundPos: UI 요소의 위치 및 크기 정의
// ------------------------------------------------------------
namespace BackGroundPos {

// 🟩 하단 메인 UI 바
namespace MainBar {
    const Vec2 MainBarPosOffset = { 750.0f, 600.0f };
    const Vec2 MainBarSize      = { 200.0f,  50.0f };

    namespace Hp {
        const Vec2 HpBarPosOffset     = { 774.0f, 619.0f };
        const Vec2 HpBarSize          = { 169.0f,  10.0f };

        const Vec2 Monster_HpBarPosBackGroundOffset = { -36.0f, -41.0f };
        const Vec2 Monster_HpBarBackGroundSize      = {  81.0f,  10.0f };

        const Vec2 Monster_HpBarPosOffset = { -35.0f, -40.0f };
        const Vec2 Monster_HpBarSize      = {  80.0f,   9.0f };
    }

    namespace Mp {
        const Vec2 MpBarPosOffset = { 774.0f, 631.0f };
        const Vec2 MpBarSize      = { 169.0f,  10.0f };
    }

    namespace Lv {
        const Vec2 LvPosOffset = { 795.0f, 610.0f };
    }

    namespace Name {
        const Vec2 NamePosOffset = { 825.0f, 598.0f };
        namespace PlayerName {
            const Vec2 NamePosOffset = { -40.0f, 15.0f };
            const Vec2 NameBarSize   = {  80.0f, 50.0f };
        }
    }

    namespace Damage {
        const Vec2 DamageSize = { 7.0f, 10.0f };
    }
}

// 🟦 하단 경험치 바
namespace Exp {
    const Vec2 ExpBarPosOffset = {   0.0f, 650.0f };
    const Vec2 ExpBarSize      = {1777.0f,  20.0f };
    const Vec2 ExpStringSize   = {  40.0f,  10.0f };
}

} // namespace BackGroundPos
