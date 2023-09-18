#pragma once

#include <stdint.h>
#include "iki_window_win.h"
#include "iki_color.h"

typedef struct iki_bitmap_data_t iki_bitmap_data;

#ifdef __cplusplus
extern "C" {
#endif

	void IkiLoadBitmap(const char* filename, iki_bitmap_data ** bitmapData );
	
	int32_t IkiGetBitmapWidth(const iki_bitmap_data * bitmapData);
	int32_t IkiGetBitmapHeight(const iki_bitmap_data* bitmapData);
	iki_color IkiGetBitmapData(const iki_bitmap_data * bitmapData, int32_t x, int32_t y);

	void IkiFreeBitmap(iki_bitmap_data** bitmapData);

#ifdef __cplusplus
}
#endif