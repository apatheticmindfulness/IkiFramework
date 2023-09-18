#include "iki_color.h"

iki_color IkiColorFromHex(uint32_t colorCode)
{
	const uint8_t red = (colorCode & 0xFF0000) >> 16;
	const uint8_t green = (colorCode & 0x00FF00) >> 8;
	const uint8_t blue = (colorCode & 0x0000FF);

	const iki_color color = { blue, green, red, 0xFF };
	return color;
}
