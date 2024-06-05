#include "Effect_Manager.h"

#include "GameInstance.h"

#include "Effect_Mesh.h"
#include "Tool_Effect_Point.h"
#include "Effect_Trail.h"
#include "Effect_Anim_Model.h"

IMPLEMENT_SINGLETON(CEffect_Manager);

CEffect_Manager::CEffect_Manager()
	: m_pGameInstance {CGameInstance::GetInstance() }
{
	Safe_AddRef(m_pGameInstance);
}

HRESULT CEffect_Manager::Initialize()
{
	return S_OK;
}

void CEffect_Manager::Tick(const _float& fTimeDelta)
{
	for (auto& pEffect : m_Effects)
		pEffect->Priority_Tick(fTimeDelta);

	if (m_pModel != nullptr) m_pModel->Priority_Tick(fTimeDelta);

	for (auto& pEffect : m_Effects)
		pEffect->Tick(fTimeDelta);

	if (m_pModel != nullptr) m_pModel->Tick(fTimeDelta);

	for (auto& pEffect : m_Effects)
		pEffect->Late_Tick(fTimeDelta);
	if (m_pModel != nullptr) m_pModel->Late_Tick(fTimeDelta);
}

HRESULT CEffect_Manager::Add_Effect(EFFECT_TYPE eType, void* pArg, _uint iShaderPass)
{
	if (eType == PARTICLE)
	{
		//파티클은 바로 생성 -> 사라지도록 구현
		CTool_Effect* pParticle = dynamic_cast<CTool_Effect*>(m_pGameInstance->Clone_Object(TEXT("Prototype_GameObject_Effect_Point"), pArg));
		if (nullptr == pParticle)
			return E_FAIL;

		pParticle->Set_ShaderPass(iShaderPass);
		m_Effects.emplace_back(pParticle);
	}
	else if (eType == EFFECT_IMG || eType == EFFECT_CELL)
	{
		// 텍스쳐들은 저장하고 있어야 함.
		CTool_Effect* pEffect = dynamic_cast<CTool_Effect*>(m_pGameInstance->Clone_Object(TEXT("Prototype_GameObject_Effect_Default"), pArg));
		if (nullptr == pEffect)
			return E_FAIL;

		pEffect->Set_ShaderPass(iShaderPass);
		m_Effects.emplace_back(pEffect);
	}
	else if (eType == EFFECT_MESH)
	{
		// 텍스쳐들은 저장하고 있어야 함.

		CTool_Effect* pEffect = dynamic_cast<CTool_Effect*>(m_pGameInstance->Clone_Object(TEXT("Prototype_GameObject_Effect_Mesh"), pArg));
		if (nullptr == pEffect)
			return E_FAIL;

		pEffect->Set_ShaderPass(iShaderPass);
		m_Effects.emplace_back(pEffect);
	}
	else if (eType == EFFECT_TRAIL)
	{
		CTool_Effect* pEffect = dynamic_cast<CTool_Effect*>(m_pGameInstance->Clone_Object(TEXT("Prototype_GameObject_Effect_Trail"), pArg));
		if (nullptr == pEffect)
			return E_FAIL;

		pEffect->Set_ShaderPass(iShaderPass);
		m_Effects.emplace_back(pEffect);
	}
	return S_OK;
}

HRESULT CEffect_Manager::Create_Model(void* pArg)
{
	if (m_pModel != nullptr)
		return E_FAIL;

	m_pModel = dynamic_cast<CEffect_Anim_Model*>(m_pGameInstance->Clone_Object(TEXT("Prototype_GameObject_Model"), pArg));
	if (m_pModel == nullptr)
		return E_FAIL;

	return S_OK;
}

HRESULT CEffect_Manager::Remove_Model()
{
	if (m_pModel == nullptr)
		return E_FAIL;

	Safe_Release(m_pModel);
	m_pModel = nullptr;

	return S_OK;
}

void CEffect_Manager::Remove_Effect(_int iIndex)
{
	Safe_Release(m_Effects[iIndex]);
	m_Effects.erase(m_Effects.begin() + iIndex);
}

void CEffect_Manager::All_Reset()
{
	for (auto& pEffect : m_Effects)
		pEffect->Reset();
}

HRESULT CEffect_Manager::Save(const _char* szFileName)
{
	char pFilePath[MAX_PATH] = "../../Data/Effect/";
	strcat_s(pFilePath, szFileName);

	ofstream ofs(pFilePath, ios::binary | ios::out);

	if (ofs.fail())
		return E_FAIL;

	_uint iNumObjects = m_Effects.size();
	ofs.write((_char*)&iNumObjects, sizeof(_uint));

	//이펙트들 모두 저장
	for (auto& pEffect : m_Effects)
	{
		_uint iEffectType = pEffect->Get_EffectType();
		ofs.write((_char*)&iEffectType, sizeof(_uint));

		if (iEffectType == PARTICLE)
		{
			CTool_Effect_Point* pParticle = dynamic_cast<CTool_Effect_Point*>(pEffect);
			// 파티클 옵션
			CTool_Effect_Point::EFFECT_POINT_DESC Desc = pParticle->Get_ParticleDesc();
			ofs.write((_char*)&Desc.iNumInstance, sizeof(_uint));
			ofs.write((_char*)&Desc.iParticleType, sizeof(_int));
			ofs.write((_char*)&Desc.isLoop, sizeof(_bool));
			ofs.write((_char*)&Desc.vLifeTime, sizeof(_float2));
			ofs.write((_char*)&Desc.vOffsetPos, sizeof(_float3));
			ofs.write((_char*)&Desc.vPivotPos, sizeof(_float3));
			ofs.write((_char*)&Desc.vRange, sizeof(_float3));
			ofs.write((_char*)&Desc.vSize, sizeof(_float2));
			ofs.write((_char*)&Desc.vSpeed, sizeof(_float2));
			ofs.write((_char*)&Desc.vPower, sizeof(_float2));
		}
		else if (iEffectType == EFFECT_IMG)
		{
			CEffect_Default* pTexture = dynamic_cast<CEffect_Default*>(pEffect);
			
			_uint iTextureMoveType = pTexture->Get_TextureMoveType();
			ofs.write((_char*)&iTextureMoveType, sizeof(_uint));
		}
		else if (iEffectType == EFFECT_MESH)
		{
			CEffect_Mesh* pMeshEffect = dynamic_cast<CEffect_Mesh*>(pEffect);

			//메쉬 파일 경로
			string strMeshFilePath = pMeshEffect->Get_MeshFilePath();
			_uint iMeshFilePathSize = strMeshFilePath.size() + 1;
			ofs.write((_char*)&iMeshFilePathSize, sizeof(_uint));
			ofs.write(strMeshFilePath.c_str(), iMeshFilePathSize);
		}
		else if (iEffectType == EFFECT_TRAIL)
		{
			CEffect_Trail* pTrailEffect = dynamic_cast<CEffect_Trail*>(pEffect);

			//메쉬 파일 경로
			string strMeshFilePath = pTrailEffect->Get_MeshFilePath();
			_uint iMeshFilePathSize = strMeshFilePath.size() + 1;
			ofs.write((_char*)&iMeshFilePathSize, sizeof(_uint));
			ofs.write(strMeshFilePath.c_str(), iMeshFilePathSize);

			//메쉬 타입
			_uint iTrailMoveType = pTrailEffect->Get_TrailMoveType();
			ofs.write((_char*)&iTrailMoveType, sizeof(_uint));
		}

		//파일 경로
		wstring strOrinFilePath = pEffect->Get_TextureFilePath();
		string strFilePath = "";
		strFilePath.assign(strOrinFilePath.begin(), strOrinFilePath.end());
		_uint iFilePathSize = strFilePath.size() + 1;
		ofs.write((_char*)&iFilePathSize, sizeof(_uint));
		ofs.write(strFilePath.c_str(), iFilePathSize);

		//마스크 경로
		_bool isMask = pEffect->isMask();
		ofs.write((_char*)&isMask, sizeof(_bool));

		wstring strMaskFilepath = pEffect->Get_MaskFilePath();
		string MaskFilePath = "";
		MaskFilePath.assign(strMaskFilepath.begin(), strMaskFilepath.end());
		_uint iMaskFilePathSize = MaskFilePath.size() + 1;
		ofs.write((_char*)&iMaskFilePathSize, sizeof(_uint));
		ofs.write(MaskFilePath.c_str(), iMaskFilePathSize);

		//노이즈 경로
		_bool isNoise = pEffect->isNoise();
		ofs.write((_char*)&isNoise, sizeof(_bool));

		wstring strNoiseFilePath = pEffect->Get_NoiseFilePath();
		string NoiseFilePath = "";
		NoiseFilePath.assign(strNoiseFilePath.begin(), strNoiseFilePath.end());
		_uint iNoiseFilePathSize = NoiseFilePath.size() + 1;
		ofs.write((_char*)&iNoiseFilePathSize, sizeof(_uint));
		ofs.write(NoiseFilePath.c_str(), iNoiseFilePathSize);

		//이미지 갯수
		const _uint iTextureNum = pEffect->Get_TextureNum();
		ofs.write((_char*)&iTextureNum, sizeof(_uint));

		//색상
		_float4 vColor = pEffect->Get_Color();
		ofs.write((_char*)&vColor, sizeof(_float4));

		//렌더러 타입
		_uint iRendererType = pEffect->Get_RendererType();
		ofs.write((_char*)&iRendererType, sizeof(_uint));

		//프레임 루프
		_bool isFrameLoop = pEffect->isFrameLoop();
		ofs.write((_char*)&isFrameLoop, sizeof(_bool));

		//시작 시간
		_float fStartTime = pEffect->Get_StartTime();
		ofs.write((_char*)&fStartTime, sizeof(_float));

		//끝 시간
		_float fDrationTime = pEffect->Get_DurationTime();
		ofs.write((_char*)&fDrationTime, sizeof(_float));

		//월드행렬
		const _float4x4* WorldMatrix = pEffect->m_pTransformCom->Get_WorldFloat4x4();
		ofs.write((_char*)WorldMatrix, sizeof(_float4x4));

		//쉐이더 패스
		_uint iShaderPass = pEffect->Get_ShaderPass();
		ofs.write((_char*)&iShaderPass, sizeof(_uint));
	}

	ofs.close();

	return S_OK;
}

HRESULT CEffect_Manager::Load(const _char* szFileName)
{
	Remove_All();

	char pFilePath[MAX_PATH] = "../../Data/Effect/";
	strcat_s(pFilePath, szFileName);

	ifstream ifs(pFilePath, ios::binary | ios::in);

	if (ifs.fail())
		return E_FAIL;

	_uint iNumObjects = 0;
	ifs.read((_char*)&iNumObjects, sizeof(_uint));

	//이펙트들 모두 저장
	for (size_t i = 0; i < iNumObjects; ++i)
	{
		_uint iEffectType = 0;
		ifs.read((_char*)&iEffectType, sizeof(_uint));

		if (iEffectType == PARTICLE)
		{
			// 파티클 옵션
			CTool_Effect_Point::EFFECT_POINT_DESC Desc{};
			ifs.read((_char*)&Desc.iNumInstance, sizeof(_uint));
			ifs.read((_char*)&Desc.iParticleType, sizeof(_int));
			ifs.read((_char*)&Desc.isLoop, sizeof(_bool));
			ifs.read((_char*)&Desc.vLifeTime, sizeof(_float2));
			ifs.read((_char*)&Desc.vOffsetPos, sizeof(_float3));
			ifs.read((_char*)&Desc.vPivotPos, sizeof(_float3));
			ifs.read((_char*)&Desc.vRange, sizeof(_float3));
			ifs.read((_char*)&Desc.vSize, sizeof(_float2));
			ifs.read((_char*)&Desc.vSpeed, sizeof(_float2));
			ifs.read((_char*)&Desc.vPower, sizeof(_float2));
			Desc.iEffectType = iEffectType;

			//파일 경로
			_char strFilePath[MAX_PATH] = "";
			_uint iFilePathSize = 0;
			ifs.read((_char*)&iFilePathSize, sizeof(_uint));
			ifs.read(strFilePath, iFilePathSize);
			strcpy_s(Desc.strTextureFilePath, strFilePath);

			//마스크 경로
			_bool isMask = false;
			ifs.read((_char*)&isMask, sizeof(_bool));
			Desc.isMask = isMask;

			_char szMaskFilePath[MAX_PATH] = "";
			_uint iMaskFilePathSize = 0;
			ifs.read((_char*)&iMaskFilePathSize, sizeof(_uint));
			ifs.read(szMaskFilePath, iMaskFilePathSize);
			Desc.strMaskFilePath = szMaskFilePath;

			//노이즈 경로
			_bool isNoise = false;
			ifs.read((_char*)&isNoise, sizeof(_bool));
			Desc.isNoise = isNoise;

			_char szNoiseFilePath[MAX_PATH] = "";
			_uint iNoiseFilePathSize = 0;
			ifs.read((_char*)&iNoiseFilePathSize, sizeof(_uint));
			ifs.read(szNoiseFilePath, iNoiseFilePathSize);
			Desc.strNoiseFilePath = szNoiseFilePath;

			//이미지 갯수
			_uint iTextureNum = 0.f;
			ifs.read((_char*)&iTextureNum, sizeof(_uint));
			Desc.iTextureNum = iTextureNum;

			//색상
			_float4 vColor = {};
			ifs.read((_char*)&vColor, sizeof(_float4));
			Desc.vColor = vColor;

			//렌더러 타입
			_uint iRendererType = 0;
			ifs.read((_char*)&iRendererType, sizeof(_uint));
			Desc.iRendererType = iRendererType;

			//프레임 루프
			_bool isFrameLoop = false;
			ifs.read((_char*)&isFrameLoop, sizeof(_bool));
			Desc.isFrameLoop = isFrameLoop;

			//시작 시간
			_float fStartTime = 0.f;
			ifs.read((_char*)&fStartTime, sizeof(_float));
			Desc.fStartTime = fStartTime;

			//끝 시간
			_float fDrationTime = 0.f;
			ifs.read((_char*)&fDrationTime, sizeof(_float));
			Desc.fDurationTime = fDrationTime;

			//월드행렬
			_float4x4 WorldMatrix = {};
			ifs.read((_char*)&WorldMatrix, sizeof(_float4x4));
			Desc.WorldMatrix = XMLoadFloat4x4(&WorldMatrix);

			//쉐이더 패스
			_uint iShaderPass = 0;
			ifs.read((_char*)&iShaderPass, sizeof(_uint));

			Desc.iEffectType = iEffectType;

			Desc.isFileLoad = true;

			Add_Effect(EFFECT_TYPE(iEffectType), &Desc, iShaderPass);

		}
		else if (iEffectType == EFFECT_IMG || iEffectType == EFFECT_CELL)
		{
			CEffect_Default::EFFECT_DEFAULT_DESC Desc{};

			_uint iTextureMoveType = 0;
			ifs.read((_char*)&iTextureMoveType, sizeof(_uint));
			Desc.iTextureMoveType = iTextureMoveType;

			//파일 경로
			_char strFilePath[MAX_PATH] = "";
			_uint iFilePathSize = 0;
			ifs.read((_char*)&iFilePathSize, sizeof(_uint));
			ifs.read(strFilePath, iFilePathSize);
			strcpy_s(Desc.strTextureFilePath, strFilePath);

			//마스크 경로
			_bool isMask = false;
			ifs.read((_char*)&isMask, sizeof(_bool));
			Desc.isMask = isMask;

			_char szMaskFilePath[MAX_PATH] = "";
			_uint iMaskFilePathSize = 0;
			ifs.read((_char*)&iMaskFilePathSize, sizeof(_uint));
			ifs.read(szMaskFilePath, iMaskFilePathSize);
			Desc.strMaskFilePath = szMaskFilePath;

			//노이즈 경로
			_bool isNoise = false;
			ifs.read((_char*)&isNoise, sizeof(_bool));
			Desc.isNoise = isNoise;

			_char szNoiseFilePath[MAX_PATH] = "";
			_uint iNoiseFilePathSize = 0;
			ifs.read((_char*)&iNoiseFilePathSize, sizeof(_uint));
			ifs.read(szNoiseFilePath, iNoiseFilePathSize);
			Desc.strNoiseFilePath = szNoiseFilePath;

			//이미지 갯수
			_uint iTextureNum = 0.f;
			ifs.read((_char*)&iTextureNum, sizeof(_uint));
			Desc.iTextureNum = iTextureNum;

			//색상
			_float4 vColor = {};
			ifs.read((_char*)&vColor, sizeof(_float4));
			Desc.vColor = vColor;

			//렌더러 타입
			_uint iRendererType = 0;
			ifs.read((_char*)&iRendererType, sizeof(_uint));
			Desc.iRendererType = iRendererType;

			//프레임 루프
			_bool isFrameLoop = false;
			ifs.read((_char*)&isFrameLoop, sizeof(_bool));
			Desc.isFrameLoop = isFrameLoop;

			//시작 시간
			_float fStartTime = 0.f;
			ifs.read((_char*)&fStartTime, sizeof(_float));
			Desc.fStartTime = fStartTime;

			//끝 시간
			_float fDrationTime = 0.f;
			ifs.read((_char*)&fDrationTime, sizeof(_float));
			Desc.fDurationTime = fDrationTime;

			//월드행렬
			_float4x4 WorldMatrix = {};
			ifs.read((_char*)&WorldMatrix, sizeof(_float4x4));
			Desc.WorldMatrix = XMLoadFloat4x4(&WorldMatrix);

			//쉐이더 패스
			_uint iShaderPass = 0;
			ifs.read((_char*)&iShaderPass, sizeof(_uint));

			Desc.iEffectType = iEffectType;

			Desc.isFileLoad = true;

			Add_Effect(EFFECT_TYPE(iEffectType), &Desc, iShaderPass);
		}
		else if (iEffectType == EFFECT_TRAIL)
		{
			CEffect_Trail::EFFECT_MESH_DESC Desc{};

			//메쉬 파일 경로
			_char szMeshFilePath[MAX_PATH] = "";
			_uint iMeshFilePathSize = 0;
			ifs.read((_char*)&iMeshFilePathSize, sizeof(_uint));
			ifs.read(szMeshFilePath, iMeshFilePathSize);
			strcpy_s(Desc.szModelFilePath, szMeshFilePath);

			//메쉬 타입
			_uint iTrailMoveType =0;
			ifs.read((_char*)&iTrailMoveType, sizeof(_uint));
			Desc.iTrailType = iTrailMoveType;

			//파일 경로
			_char strFilePath[MAX_PATH] = "";
			_uint iFilePathSize = 0;
			ifs.read((_char*)&iFilePathSize, sizeof(_uint));
			ifs.read(strFilePath, iFilePathSize);
			strcpy_s(Desc.strTextureFilePath, strFilePath);

				//마스크 경로
			_bool isMask = false;
			ifs.read((_char*)&isMask, sizeof(_bool));
			Desc.isMask = isMask;

			_char szMaskFilePath[MAX_PATH] = "";
			_uint iMaskFilePathSize = 0;
			ifs.read((_char*)&iMaskFilePathSize, sizeof(_uint));
			ifs.read(szMaskFilePath, iMaskFilePathSize);
			Desc.strMaskFilePath = szMaskFilePath;

				//노이즈 경로
			_bool isNoise = false;
			ifs.read((_char*)&isNoise, sizeof(_bool));
			Desc.isNoise = isNoise;

			_char szNoiseFilePath[MAX_PATH] = "";
			_uint iNoiseFilePathSize = 0;
			ifs.read((_char*)&iNoiseFilePathSize, sizeof(_uint));
			ifs.read(szNoiseFilePath, iNoiseFilePathSize);
			Desc.strNoiseFilePath = szNoiseFilePath;

			//이미지 갯수
			_uint iTextureNum = 0.f;
			ifs.read((_char*)&iTextureNum, sizeof(_uint));
			Desc.iTextureNum = iTextureNum;

			//색상
			_float4 vColor = {};
			ifs.read((_char*)&vColor, sizeof(_float4));
			Desc.vColor = vColor;

			//렌더러 타입
			_uint iRendererType = 0;
			ifs.read((_char*)&iRendererType, sizeof(_uint));
			Desc.iRendererType = iRendererType;

			//프레임 루프
			_bool isFrameLoop = false;
			ifs.read((_char*)&isFrameLoop, sizeof(_bool));
			Desc.isFrameLoop = isFrameLoop;

			//시작 시간
			_float fStartTime = 0.f;
			ifs.read((_char*)&fStartTime, sizeof(_float));
			Desc.fStartTime = fStartTime;

			//끝 시간
			_float fDrationTime = 0.f;
			ifs.read((_char*)&fDrationTime, sizeof(_float));
			Desc.fDurationTime = fDrationTime;

			//월드행렬
			_float4x4 WorldMatrix = {};
			ifs.read((_char*)&WorldMatrix, sizeof(_float4x4));
			Desc.WorldMatrix = XMLoadFloat4x4(&WorldMatrix);

			//쉐이더 패스
			_uint iShaderPass = 0;
			ifs.read((_char*)&iShaderPass, sizeof(_uint));

			Desc.iEffectType = iEffectType;

			Desc.isFileLoad = true;

			Add_Effect(EFFECT_TYPE(iEffectType), &Desc, iShaderPass);
		}

	}

	ifs.close();
	return S_OK;
}

void CEffect_Manager::Remove_All()
{
	for (auto& pEffect : m_Effects)
		Safe_Release(pEffect);

	m_Effects.clear();
}

void CEffect_Manager::Release_EffectManager()
{
	CEffect_Manager::DestroyInstance();
}

void CEffect_Manager::Free()
{
	Safe_Release(m_pGameInstance);

	for (auto& pEffect : m_Effects)
		Safe_Release(pEffect);
	m_Effects.clear();

	Safe_Release(m_pModel);
}
