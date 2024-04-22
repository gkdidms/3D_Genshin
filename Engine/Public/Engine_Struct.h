#ifndef Engine_Struct_h__
#define Engine_Struct_h__

namespace Engine
{
	/* 엔진을 초기화하기위해 필요한 데이터 여러개를 묶었다. */
	typedef struct
	{
		bool			isWindowed;
		HWND			hWnd;
		HINSTANCE		hInst;
		unsigned int	iWinSizeX;
		unsigned int	iWinSizeY;
	}ENGINE_DESC;

	typedef struct ENGINE_DLL tVexPos
	{
		XMFLOAT3 vPosition;

		static const _uint iNumElements = 1;
		static const D3D11_INPUT_ELEMENT_DESC Elements[iNumElements];
	} VXTPOS;
	
	typedef struct ENGINE_DLL tVexPosTex 
	{
		XMFLOAT3 vPosition;
		XMFLOAT2 vTexcoord;

		static const _uint iNumElements = 2;
		static const D3D11_INPUT_ELEMENT_DESC Elements[iNumElements];
	} VXTPOSTEX;

	typedef struct ENGINE_DLL tVexNorTex
	{
		XMFLOAT3 vPosition;
		XMFLOAT3 vNormal;
		XMFLOAT2 vTexcoord;

		static const _uint iNumElements = 3;
		static const D3D11_INPUT_ELEMENT_DESC Elements[iNumElements];
	} VTXNORTEX;

	typedef struct ENGINE_DLL tVexMesh
	{
		XMFLOAT3 vPosition;
		XMFLOAT3 vNormal;
		XMFLOAT2 vTexcoord;
		XMFLOAT3 vTangent;

		static const _uint iNumElements = 4;
		static const D3D11_INPUT_ELEMENT_DESC Elements[iNumElements];
	} VTXMESH;

	typedef struct ENGINE_DLL tVexAnimMesh
	{
		XMFLOAT3 vPosition;
		XMFLOAT3 vNormal;
		XMFLOAT2 vTexcoord;
		XMFLOAT3 vTangent;

		XMUINT4 vBlendIndices; // 정점에게 영향을 주는 뼈의 인덱스
		XMFLOAT4 vBlendWeights; // 정점에게 영향을 주는 뼈의 영향을 주는 정도 (가중치)

		static const _uint iNumElements = 6;
		static const D3D11_INPUT_ELEMENT_DESC Elements[iNumElements];
	} VTXANIMMESH;

	typedef struct
	{
		class CTexture* pMaterialTextures[AI_TEXTURE_TYPE_MAX];
	} MESH_MATERIAL;

	typedef struct
	{
		XMFLOAT3 m_vScale;
		XMFLOAT4 vRotate;
		XMFLOAT3 vPosition;

		double Time;
	} KEYFRAME;
}
#endif // Engine_Struct_h__
