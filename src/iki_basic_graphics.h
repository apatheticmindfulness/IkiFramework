#pragma once
#include "iki_color.h"
#include "iki_window_win.h"

#ifdef __cplusplus
extern "C" {
#endif

uint8_t		IkiUseBasicGraphics(int32_t width, int32_t height, int32_t bitCount, uint8_t keepAspectRatio);
uint8_t		IkiEndBasicGraphics(void);

void		IkiRenderStart(void);
void		IkiSetPixel(int32_t x, int32_t y, iki_color color);
void		IkiRenderEnd(void);
void		IkiClearBackground(iki_color color);

float		IkiGetCurrentCanvasWidth(void);
float		IkiGetCurrentCanvasHeight(void);
float		IkiGetCurrentCanvasX(void);
float		IkiGetCurrentCanvasY(void);

#ifdef __cplusplus
}
#endif