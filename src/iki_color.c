#include "iki_color.h"

iki_color IkiColorFromHex(uint32_t colorCode)
{
	const uint8_t red = (colorCode & 0xFF0000) >> 16;
	const uint8_t green = (colorCode & 0x00FF00) >> 8;
	const uint8_t blue = (colorCode & 0x0000FF);

	const iki_color color = { blue, green, red, 0xFF };
	return color;
}

uint8_t IkiIsColorEqual(iki_color color1, iki_color color2)
{
	return color1.r == color2.r &&
		color1.g == color2.g &&
		color1.b == color2.b &&
		color1.a == color2.a;
}
