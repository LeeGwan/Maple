#pragma once

#include "../../../../Utils/Vector/Vector.h"
#define COL32_R_SHIFT    0
#define COL32_G_SHIFT    8
#define COL32_B_SHIFT    16
#define COL32_A_SHIFT    24
#define COL32_A_MASK     0xFF000000
static inline float ColSaturate(float f) { return (f < 0.0f) ? 0.0f : (f > 1.0f) ? 1.0f : f; }
#define F32_TO_INT8_SAT(_VAL) ((int)(ColSaturate(_VAL) * 255.0f + 0.5f))
class Color
{
	float x, y, z, w;

public:
	Color() {};
	Color(BYTE r, BYTE g, BYTE b, BYTE a = 255) { float sc = 1.0f / 255.0f; x = (float)r * sc; y = (float)g * sc; z = (float)b * sc; w = (float)a * sc; }
	Color(uint32_t rgba) { float sc = 1.0f / 255.0f; x = (float)((rgba >> COL32_R_SHIFT) & 0xFF) * sc; y = (float)((rgba >> COL32_G_SHIFT) & 0xFF) * sc; z = (float)((rgba >> COL32_B_SHIFT) & 0xFF) * sc; w = (float)((rgba >> COL32_A_SHIFT) & 0xFF) * sc; }

	operator uint32_t() const
	{
		uint32_t out;
		out = ((uint32_t)F32_TO_INT8_SAT(x)) << COL32_R_SHIFT;
		out |= ((uint32_t)F32_TO_INT8_SAT(y)) << COL32_G_SHIFT;
		out |= ((uint32_t)F32_TO_INT8_SAT(z)) << COL32_B_SHIFT;
		out |= ((uint32_t)F32_TO_INT8_SAT(w)) << COL32_A_SHIFT;
		return out;
	}
};

namespace Colors
{
	namespace Simple
	{
		const Color stasis{ 112, 128, 147, 255 };
		const Color dark_red{ 255,   0,   0, 150 };
		const Color rudy{ 255,	 0,	 40, 255 };
		const Color lime{ 200, 236,  70, 255 };
		const Color drop_shadow{ 0,   0,   0, 150 };
		const Color pacific_blue{ 13, 165, 206, 255 };
		const Color exp_bar{ 140, 90, 226, 255 };
		const Color exp_bar_bg{ 20, 20, 20, 160 };

		namespace Legacy
		{
			const Color background{ 0,   0,   0, 180 };	// (background/border) black 70%
			const Color foreground{ 120, 225, 120, 230 };	// (filling no cd) greenish
			const Color White_font{ 255, 255, 255, 255 };	// (cd font) white
			const Color black{ 0,   0,   0, 255 };	// (level points) black
			const Color orange{ 230, 145,  30, 255 };	// (progressbar) orange
			const Color red{ 255,  27, 18, 255 };
			const Color border{ 68,  68, 68, 255 };	// grey
		}
		namespace Modern
		{
			const Color border{ 68,  68, 68, 255 };	// grey
			const Color background{ 34,  31, 27, 255 };  // darker grey (shadow)
			const Color green{ 124, 227,  9, 255 };	// green
			const Color red{ 255,  27, 18, 255 };	// red
		}
	}

}
namespace BackGroundPos
{
	namespace MainBar
	{
		const Vec2 MainBarPosOffset = { 750.0f, 600.0f };
		const Vec2 MainBarSize = { 200.0f,  50.0f };
		namespace Hp
		{
			const Vec2 HpBarPosOffset = { 774.0f, 619.0f };
			const Vec2 HpBarSize = { 169.0f,  10.0f };

			const Vec2 Monster_HpBarPosBackGroundOffset = { -36.0f,  -41.0f };
			const Vec2 Monster_HpBarBackGroundSize = { 81.0f,  10.0f };

			const Vec2 Monster_HpBarPosOffset = { -35.0f,  -40.0f };
			const Vec2 Monster_HpBarSize = { 80.0f,  9.0f };
		}
		namespace Mp
		{
			const Vec2 MpBarPosOffset = { 774.0f, 631.0f };
			const Vec2 MpBarSize = { 169.0f,  10.0f };
		}
		namespace Lv
		{
			const Vec2 LvPosOffset = { 795.0f, 610.0f };
		}
		namespace Name
		{
			const Vec2 NamePosOffset = { 825.0f, 598.0f };

			namespace PlayerName
			{
				const Vec2 NamePosOffset = { -40.0f, 15.0f };
				const Vec2 NameBarSize = { 80.0f, 50.0f };
			}
		}
		namespace Damage
		{
			const Vec2 DamageSize = { 7.0f, 10.0f };
		}
	}
	namespace Exp
	{
		const Vec2 ExpBarPosOffset = { 0.0f, 650.0f };
		const Vec2 ExpBarSize = { 1777.0f,  20.0f };
		const Vec2 ExpStringSize = { 40.0f,  10.0f };
	}
}