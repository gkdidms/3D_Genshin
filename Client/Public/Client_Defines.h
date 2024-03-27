#pragma once

#include "process.h"

namespace Client {
	enum LEVEL_STATE { LEVEL_STATIC, LEVEL_LOADING, LEVEL_LOGO, LEVEL_GAMEPLAY, LEVEL_END};

	const unsigned int g_iWinSizeX = 1280;
	const unsigned int g_iWinSizeY = 720;
};

extern HWND g_hWnd;
extern HINSTANCE g_hInst;

using namespace Client;