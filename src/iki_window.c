#include "iki_window.h"
#include <malloc.h>

typedef enum iki_keyboard_enum_t
{
	IKI_KEYBOARD_NONE,
	IKI_KEYBOARD_PRESSED,
	IKI_KEYBOARD_RELEASED
} iki_keyboard_enum;

typedef struct iki_keyboard_struct_t
{
	iki_keyboard_enum state;
	uint8_t autoRepeat;
	uint8_t keyCode[256];
} iki_keyboard_struct;

typedef enum iki_mouse_enum_t
{
	IKI_MOUSE_NONE,
	IKI_MOUSE_CLICKED,
	IKI_MOUSE_RELEASED
} iki_mouse_enum;

typedef struct iki_mouse_struct_t
{
	int32_t				x;
	int32_t				y;
	iki_mouse_enum		state;
} iki_mouse_struct;

typedef struct iki_window_struct_t
{
	int32_t width;
	int32_t height;
	int32_t activeWidth;
	int32_t activeHeight;
	uint8_t created;
	uint8_t closed;
	uint8_t showed;

	// Keyboard
	iki_keyboard_struct keyboard;

	// Mouse
	iki_mouse_struct mouse;

	// Win32
	WNDCLASSEX winClass;
	HWND winHandle;
	MSG winMessage;
	DWORD winFlags;

} iki_window_struct;

static iki_window_struct		g_window;
static LRESULT CALLBACK			MainWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
static inline void				ClearInputState(void);

uint8_t IkiCreateWindow(const char* title, const int width, const int height)
{
	if (!g_window.created)
	{
		g_window.width = width;
		g_window.height = height;
		
		// Create window win32 class
		g_window.winClass.cbSize = sizeof(WNDCLASSEX);
		g_window.winClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
		g_window.winClass.lpfnWndProc = MainWindowProc;
		g_window.winClass.hInstance = GetModuleHandle(0);
		g_window.winClass.hCursor = LoadCursor(0, IDC_ARROW);
		g_window.winClass.lpszMenuName = 0;
		g_window.winClass.lpszClassName = "IkiFrameworkWindow";
		g_window.winClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);

		if (!RegisterClassEx(&g_window.winClass))
		{
			// TODO : Error log
			return false;
		}

		DWORD windowFlags = WS_SYSMENU | WS_CAPTION | WS_MINIMIZEBOX | g_window.winFlags;

		RECT rc = { 0, 0, (LONG)g_window.width, (LONG)g_window.height };
		if (!AdjustWindowRect(&rc, windowFlags, false))
		{
			// TODO : Error log
			return false;
		}

		// Create window handle 
		g_window.winHandle = CreateWindow(
			g_window.winClass.lpszClassName,
			title,
			windowFlags,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			rc.right - rc.left,
			rc.bottom - rc.top,
			0,
			0,
			GetModuleHandle(0),
			0);

		if (!g_window.winHandle)
		{
			// TODO : Error log
			return false;
		}

		g_window.created = true;
	}

	return true;
}

void IkiCloseWindow(void)
{
	if (g_window.created)
	{
		g_window.closed = true;
	}
}

void IkiAddWindowFlags(DWORD flags)
{
	g_window.winFlags |= flags;
}

uint8_t IkiRunProcess(void)
{
	while (PeekMessage(&g_window.winMessage, 0, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&g_window.winMessage);
		DispatchMessage(&g_window.winMessage);

		if (g_window.winMessage.message == WM_QUIT || g_window.closed || !g_window.showed)
		{
			g_window.created = false;
			return false;
		}
	}

	return true;
}

void* IkiGetWindowHandle(void)
{
	if (g_window.winHandle)
	{
		return g_window.winHandle;
	}

	return 0;
}

void IkiShowWindow(void)
{
	g_window.showed = true;
	if (ShowWindow(g_window.winHandle, SW_SHOW))
	{
		return;
	}
	return;
}

int32_t	IkiGetCurrentWindowWidth(void)
{
	return g_window.activeWidth;
}

int32_t	IkiGetCurrentWindowHeight(void)
{
	return g_window.activeHeight;
}

uint8_t	IkiKeyboardIsPressed(uint32_t key_code)
{
	return g_window.keyboard.keyCode[key_code];
}

uint8_t IkiKeyboardIsReleased(uint32_t key_code)
{
	if (g_window.keyboard.state == IKI_KEYBOARD_RELEASED)
	{
		return g_window.keyboard.keyCode[key_code] == false; // return true
	}

	return false;
}

uint8_t	IkiMouseLeftIsPressed(void)
{
	return g_window.mouse.state == IKI_MOUSE_CLICKED;
}

uint8_t	IkiMouseLeftIsReleased(void)
{
	if (g_window.mouse.state == IKI_MOUSE_RELEASED)
	{
		return true;
	}
	return false;
}

int32_t	IkiGetMouseX(void)
{
	return g_window.mouse.x;
}

int32_t	IkiGetMouseY(void)
{
	return g_window.mouse.y;
}

#define KEYBOARD_STATE(STATE, STATUS) \
g_window.keyboard.state = STATE; \
g_window.keyboard.keyCode[keyCode] = STATUS

#define MOUSE_STATE(STATE, MOUSE_POINT) \
g_window.mouse.state = STATE; \
g_window.mouse.x = (int32_t)MOUSE_POINT.x; \
g_window.mouse.y = (int32_t)MOUSE_POINT.y;

LRESULT CALLBACK MainWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC dc;

	switch (uMsg)
	{
		case WM_DESTROY:
		{
			PostQuitMessage(0);
		}
		break;

		case WM_PAINT:
		{
			dc = BeginPaint(hwnd, &ps);
			EndPaint(hwnd, &ps);
		}
		break;

		case WM_SIZE:
		{
			RECT rect;
			GetClientRect(g_window.winHandle, &rect);
			g_window.activeWidth = rect.right - rect.left;
			g_window.activeHeight = rect.bottom - rect.top;
		}
		break;

		case WM_KILLFOCUS:
		{
			ClearInputState();
		}
		break;

		// Keyboard
		case WM_SYSKEYDOWN:
		case WM_KEYDOWN:
		{
			const uint32_t keyCode = (uint32_t)wParam;
			const uint32_t keyIsDown = (uint32_t)(lParam & (1 << 30));
			if (keyIsDown == 0u) // No auto repeat
			{
				KEYBOARD_STATE(IKI_KEYBOARD_PRESSED, true);
			}
		}
		break;

		case WM_SYSKEYUP:
		case WM_KEYUP:
		{
			const uint32_t keyCode = (uint32_t)wParam;
			KEYBOARD_STATE(IKI_KEYBOARD_RELEASED, false);
		}
		break;

		// Mouse input
		case WM_LBUTTONDOWN:
		{
			const POINTS point = MAKEPOINTS(lParam);
			MOUSE_STATE(IKI_MOUSE_CLICKED, point);
		}
		break;

		case WM_LBUTTONUP:
		{
			const POINTS point = MAKEPOINTS(lParam);
			MOUSE_STATE(IKI_MOUSE_RELEASED, point);
		}
		break;

		case WM_MOUSEMOVE:
		{
			if (wParam & MK_LBUTTON)
			{
				const POINTS point = MAKEPOINTS(lParam);
				MOUSE_STATE(IKI_MOUSE_CLICKED, point);
			}
		}
		break;
	}

	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

void ClearInputState(void)
{
	g_window.keyboard.state = IKI_KEYBOARD_NONE;
	memset(g_window.keyboard.keyCode, 0, 256);

	g_window.mouse.state = IKI_MOUSE_NONE;
	g_window.mouse.x = 0;
	g_window.mouse.y = 0;
}