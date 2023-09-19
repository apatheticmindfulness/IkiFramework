

#include "iki_window.h"
#include "iki_basic_graphics.h"

// Window size
#define WINDOW_WIDTH 600
#define WINDOW_HEIGHT 400

// Pixel size
// It doesn't have to be the same as The Window Size.
#define CANVAS_WIDTH WINDOW_WIDTH
#define CANVAS_HEIGHT WINDOW_HEIGHT

int main()
{
	// Optional.
	DWORD windowFlags = WS_SYSMENU | WS_CAPTION | WS_MINIMIZEBOX;
	IkiAddWindowFlags(windowFlags);

	// Create window
	IkiCreateWindow("IkiFramework", WINDOW_WIDTH, WINDOW_HEIGHT);

	// Tell the window to use the basic graphics functionalities.
	IkiUseBasicGraphics(CANVAS_WIDTH, CANVAS_HEIGHT, 32, false);

	IkiShowWindow();
	UpdateWindow((HWND)IkiGetWindowHandle());

	while (IkiRunProcess())
	{
		IkiRenderStart();

		// Draw a pixel at coordinates (10, 10).
		IkiSetPixel(10, 10, IkiColorFromHex(0xcf222e));

		IkiRenderEnd();
	}

	return 0;
}