#pragma once
#include <stdint.h>
#include <stdbool.h>
#include "iki_window_win.h"

#ifdef __cplusplus
extern "C" {
#endif

uint8_t		IkiCreateWindow(const char* title, const int width, const int height);
void		IkiCloseWindow(void);
void 		IkiAddWindowFlags(DWORD flags);
uint8_t		IkiRunProcess(void);
void *		IkiGetWindowHandle(void);
void		IkiShowWindow(void);

int32_t		IkiGetCurrentWindowWidth(void);
int32_t		IkiGetCurrentWindowHeight(void);

uint8_t		IkiKeyboardIsPressed(uint32_t key_code);
uint8_t		IkiKeyboardIsReleased(uint32_t key_code);

uint8_t		IkiMouseLeftIsPressed(void);
uint8_t		IkiMouseLeftIsReleased(void);
int32_t		IkiGetMouseX(void);
int32_t		IkiGetMouseY(void);


#ifdef __cplusplus
}
#endif