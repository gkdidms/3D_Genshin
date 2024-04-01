#pragma once

#include <d3d11.h>
#include "dinput.h"
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <DirectXCollision.h>
#include <string>
#include "fx11/d3dx11effect.h"
#include "DirectXTK/DDSTextureLoader.h"
#include "DirectXTK/WICTextureLoader.h"
#include "DirectXTK/Keyboard.h"
#include "DirectXTK/Mouse.h"
#include "fmode/inc/fmod.h"
#pragma comment(lib, "fmod_vc.lib")

#include "assimp/scene.h"
#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"

using namespace DirectX;
using namespace TriangleTests;

#include <vector>
#include <list>
#include <map>
#include <algorithm>

#include <iostream>
#include <io.h>
#include <fstream>
#include <sstream>

#include "Engine_Typedef.h"
#include "Engine_Function.h"
#include "Engine_Macro.h"
#include "Engine_Struct.h"

//#ifdef _DEBUG
//
//#define _CRTDBG_MAP_ALLOC
//#include <stdlib.h>
//#include <crtdbg.h>
//
//#ifndef DBG_NEW 
//
//#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ ) 
//#define new DBG_NEW 
//
//#endif
//
//#endif // _DEBUG

namespace Engine
{
	enum MOUSEKEYSTATE { DIM_LB, DIM_RB, DIM_MB, DIM_END };
	enum MOUSEMOVESTATE { DIMS_X, DIMS_Y, DIMS_Z, DIMS_END };
}

using namespace Engine;
using namespace std;


enum CHANNELID {
	SOUND_EFFECT,
	SOUND_BGM,
	SOUND_END
};
