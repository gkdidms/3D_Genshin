#pragma once

namespace Tool
{
	enum LEVEL_STATE { LEVEL_MAIN, LEVEL_END };

	const unsigned int g_iWinSizeX = 1280;
	const unsigned int g_iWinSizeY = 720;
}

#include <iostream>
#include <io.h>

extern HWND g_hWnd;
extern HINSTANCE g_hInst;

using namespace Tool;