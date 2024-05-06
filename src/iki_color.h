#pragma once
#include <stdint.h>

typedef struct iki_color_t
{
	uint8_t b;
	uint8_t g;
	uint8_t r;
	uint8_t a;
} iki_color;

#ifdef __cplusplus
extern "C" {
#endif

iki_color	IkiColorFromHex(uint32_t colorCode);
uint8_t		IkiIsColorEqual(iki_color color1, iki_color color2);

#ifdef __cplusplus
}
#endif

// BGRA order
#define COLOR_BLACK				{ 0x00, 0x00, 0x00, 0x00 }
#define COLOR_WHITE				{ 0xFF, 0xFF, 0xFF, 0xFF }
#define COLOR_GRAY				{ 0x80, 0x80, 0x80, 0xFF }
#define COLOR_BLUE				{ 0xFF, 0x00, 0x00, 0xFF }
#define COLOR_GREEN				{ 0x00, 0xFF, 0x00, 0xFF }
#define COLOR_RED				{ 0x00, 0x00, 0xFF, 0xFF }
#define COLOR_YELLOW			{ 0x00, 0xFF, 0xFF, 0xFF }
#define COLOR_PURPLE			{ 0x80, 0x00, 0x80, 0xFF }
#define COLOR_CYAN				{ 0xFF, 0xFF, 0x00, 0xFF }
#define COLOR_CORNFLOWERBLUE	{ 0xED, 0x95, 0x64, 0xFF }