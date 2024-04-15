#pragma once

#include "process.h"

namespace Client {
	enum LEVEL_STATE { LEVEL_STATIC, LEVEL_LOADING, LEVEL_LOGO, LEVEL_GAMEPLAY, LEVEL_END};

    enum PLAYER_STATE {
        PLAYER_IDLE,
        PLAYER_ATTACK_IDLE,
        PLAYER_IDLE_PUT_AWAY,
        PLAYER_RUN_START,
        PLAYER_RUN,
        PLAYER_RUN_STOP,
        PLAYER_SPRINT_START,
        PLAYER_SPRINT,
        PLAYER_SPRINT_TO_RUN,
        PLAYER_SPRINT_STOP,
        PLAYER_JUMP,
        PLAYER_FLY,
        PLAYER_ATTACK_1,
        PLAYER_ATTACK_2,
        PLAYER_ATTACK_3,
        PLAYER_ATTACK_4,
        PLAYER_ELEMENTAL_1,
        PLAYER_ELEMENTAL_2,
        PLAYER_ELEMENTAL_END,
        PLAYER_ELENENTAL_BURST,
        PLAYER_ELENENTAL_BURST_END,
        PLAYER_END
    };

	const unsigned int g_iWinSizeX = 1280;
	const unsigned int g_iWinSizeY = 720;
};

extern HWND g_hWnd;
extern HINSTANCE g_hInst;

using namespace Client;