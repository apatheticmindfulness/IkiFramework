#include "iki_basic_graphics.h"
#include "iki_window.h"
#include <malloc.h>
#include <math.h>
#include <assert.h>

typedef struct iki_basic_graphics_struct_t
{
	int32_t		bitCount;
	int32_t		width;
	int32_t		height;
	size_t		totalCanvasSize;

	// These variable will change according to the window size
	float		currentWidth;
	float		currentHeight;
	float		currentX;
	float		currentY;

	uint8_t		keepAspectRatio;

	BITMAPINFO	winBitmapInfo;
	iki_color* colorMemory;
} iki_basic_graphics_struct;

static iki_basic_graphics_struct		g_graphics;

uint8_t IkiUseBasicGraphics(int32_t width, int32_t height, int32_t bitCount, uint8_t keepAspectRatio)
{
	g_graphics.width = width;
	g_graphics.height = height;
	g_graphics.bitCount = bitCount;
	g_graphics.totalCanvasSize = g_graphics.width * g_graphics.height * sizeof(iki_color);
	g_graphics.keepAspectRatio = keepAspectRatio;

	// Configure BitmapInfo
	g_graphics.winBitmapInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	g_graphics.winBitmapInfo.bmiHeader.biWidth = g_graphics.width;
	g_graphics.winBitmapInfo.bmiHeader.biHeight = -1 * (g_graphics.height);
	g_graphics.winBitmapInfo.bmiHeader.biPlanes = 1;
	g_graphics.winBitmapInfo.bmiHeader.biBitCount = g_graphics.bitCount;
	g_graphics.winBitmapInfo.bmiHeader.biCompression = BI_RGB;

	// Allocate memory
	g_graphics.colorMemory = (iki_color*)VirtualAlloc(0, g_graphics.totalCanvasSize, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
	if (!g_graphics.colorMemory)
	{
		// Log : Failed to allocate memory
		return false;
	}

	return true;
}

void IkiRenderStart(void)
{
	const size_t totalSize = g_graphics.totalCanvasSize;
	memset(g_graphics.colorMemory, 0, totalSize);
}

void IkiSetPixel(int32_t x, int32_t y, iki_color color)
{
	assert(x >= 0);
	assert(x < (int32_t)g_graphics.width);
	assert(y >= 0);
	assert(y < (int32_t)g_graphics.height);
	g_graphics.colorMemory[g_graphics.width * y + x] = color;
}

void IkiRenderEnd(void)
{
	HDC dc = GetDC(IkiGetWindowHandle());

	// Retrieve window client size
	RECT clientRect;
	GetClientRect(IkiGetWindowHandle(), &clientRect);
	int32_t clientWindowWidth = IkiGetCurrentWindowWidth();
	int32_t clientWindowHeight = IkiGetCurrentWindowHeight();

	// Calculate aspect ratio
	float targetCanvasWidth = (float)clientWindowWidth;
	float targetCanvasHeight = (float)clientWindowHeight;

	float targetCanvasX = 0.0f;
	float targetCanvasY = 0.0f;

	if (g_graphics.keepAspectRatio)
	{
		// Calculate aspect ratio
		float canvasAspectRatio = (float)g_graphics.width / (float)g_graphics.height;
		targetCanvasWidth = clientWindowHeight * canvasAspectRatio;
		targetCanvasHeight = clientWindowWidth / canvasAspectRatio;

		targetCanvasX = 0.0f;
		targetCanvasY = 0.0f;

		if (targetCanvasHeight > (float)clientWindowHeight)
		{
			targetCanvasHeight = (float)clientWindowHeight;
			targetCanvasX = ((float)clientWindowWidth / 2.0f) - (targetCanvasWidth / 2.0f);
		}
		else
		{
			targetCanvasWidth = (float)clientWindowWidth;
			targetCanvasY = ((float)clientWindowHeight / 2.0f) - (targetCanvasHeight / 2.0f);
		}
	}

	// assign the current canvas size to the global variable
	g_graphics.currentWidth = targetCanvasWidth;
	g_graphics.currentHeight = targetCanvasHeight;
	g_graphics.currentX = targetCanvasX;
	g_graphics.currentY = targetCanvasY;

	StretchDIBits(dc,
		(int32_t)targetCanvasX, (int32_t)targetCanvasY, (int32_t)targetCanvasWidth, (int32_t)targetCanvasHeight,
		0, 0, g_graphics.width, g_graphics.height,
		g_graphics.colorMemory,
		&g_graphics.winBitmapInfo,
		DIB_RGB_COLORS, SRCCOPY);

	ReleaseDC(IkiGetWindowHandle(), dc);
}

void IkiClearBackground(iki_color color)
{
	const iki_color clearBackgroundColor = color;
	for (int32_t x = 0; x < g_graphics.width * g_graphics.height; x++)
	{
		memcpy(g_graphics.colorMemory + x, &clearBackgroundColor, sizeof(iki_color));
	}
}

float IkiGetCurrentCanvasWidth()
{
	return g_graphics.currentWidth;
}

float IkiGetCurrentCanvasHeight()
{
	return g_graphics.currentHeight;
}

float IkiGetCurrentCanvasX()
{
	return g_graphics.currentX;
}

float IkiGetCurrentCanvasY()
{
	return g_graphics.currentY;
}
