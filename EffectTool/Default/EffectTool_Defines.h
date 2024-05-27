#pragma once

namespace Effect {
	enum LEVEL_STATE { LEVEL_MAIN, LEVEL_END };

	const unsigned int g_iWinSizeX = 1280;
	const unsigned int g_iWinSizeY = 720;
}

#include <iostream>
#include <io.h>
#include <fstream>;
#include <sstream>

extern HWND g_hWnd;
extern HINSTANCE g_hInst;

using namespace Effect;