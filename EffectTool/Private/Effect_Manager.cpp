#include "Effect_Manager.h"

#include "GameInstance.h"

#include "Effect_Mesh.h"
#include "Tool_Effect_Point.h"

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

	for (auto& pEffect : m_Effects)
		pEffect->Tick(fTimeDelta);

	for (auto& pEffect : m_Effects)
		pEffect->Late_Tick(fTimeDelta);
}

HRESULT CEffect_Manager::Add_Effect(EFFECT_TYPE eType, void* pArg)
{
	if (eType == PARTICLE)
	{
		//파티클은 바로 생성 -> 사라지도록 구현
		CTool_Effect* pParticle = dynamic_cast<CTool_Effect*>(m_pGameInstance->Clone_Object(TEXT("Prototype_GameObject_Effect_Point"), pArg));
		if (nullptr == pParticle)
			return E_FAIL;

		m_Effects.emplace_back(pParticle);
	}
	else if (eType == EFFECT_IMG)
	{
		// 텍스쳐들은 저장하고 있어야 함.
		CTool_Effect* pEffect = dynamic_cast<CTool_Effect*>(m_pGameInstance->Clone_Object(TEXT("Prototype_GameObject_Effect_Default"), pArg));
		if (nullptr == pEffect)
			return E_FAIL;

		m_Effects.emplace_back(pEffect);
	}
	else if (eType == EFFECT_MESH)
	{
		// 텍스쳐들은 저장하고 있어야 함.

		CTool_Effect* pEffect = dynamic_cast<CTool_Effect*>(m_pGameInstance->Clone_Object(TEXT("Prototype_GameObject_Effect_Mesh"), pArg));
		if (nullptr == pEffect)
			return E_FAIL;

		m_Effects.emplace_back(pEffect);
	}

	return S_OK;
}

void CEffect_Manager::Remove_Effect(_int iIndex)
{
	Safe_Release(m_Effects[iIndex]);
	m_Effects.erase(m_Effects.begin() + iIndex);
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

			//파티클 파일 이름
			string strName = "";
			strName.assign(pParticle->Get_FileName().begin(), pParticle->Get_FileName().end());
			_uint iFileNameSize = pParticle->Get_FileName().size() + 1;
			ofs.write((_char*)&iFileNameSize, sizeof(_uint));
			ofs.write(strName.c_str(), iFileNameSize);

			//파티클 파일 경로
			string strFilePath = "";
			strFilePath.assign(pParticle->Get_FilePath().begin(), pParticle->Get_FilePath().end());
			_uint iFilePathSize = pParticle->Get_FilePath().size() + 1;
			ofs.write((_char*)&iFilePathSize, sizeof(_uint));
			ofs.write(strFilePath.c_str(), iFilePathSize);

			//월드행렬
			const _float4x4* WorldMatrix = pParticle->m_pTransformCom->Get_WorldFloat4x4();
			ofs.write((_char*)WorldMatrix, sizeof(_float4x4));
		}
		else if (iEffectType == EFFECT_IMG)
		{
			CEffect_Default* pTextureEffect = dynamic_cast<CEffect_Default*>(pEffect);

			//텍스쳐 파일 이름
			string strName = "";
			strName.assign(pTextureEffect->Get_FileName().begin(), pTextureEffect->Get_FileName().end());
			_uint iFileNameSize = pTextureEffect->Get_FileName().size() + 1;
			ofs.write((_char*)&iFileNameSize, sizeof(_uint));
			ofs.write(strName.c_str(), iFileNameSize);

			//파일 경로
			string strFilePath = "";
			strFilePath.assign(pTextureEffect->Get_TextureFilePath().begin(), pTextureEffect->Get_TextureFilePath().end());
			_uint iFilePathSize = pTextureEffect->Get_TextureFilePath().size() + 1;
			ofs.write((_char*)&iFilePathSize, sizeof(_uint));
			ofs.write(strFilePath.c_str(), iFilePathSize);

			//월드행렬
			const _float4x4* WorldMatrix = pTextureEffect->m_pTransformCom->Get_WorldFloat4x4();
			ofs.write((_char*)WorldMatrix, sizeof(_float4x4));
		}
		else if (iEffectType == EFFECT_MESH)
		{
			CEffect_Mesh* pMeshEffect = dynamic_cast<CEffect_Mesh*>(pEffect);

			//파일 이름
			string strName = "";
			strName.assign(pMeshEffect->Get_FileName().begin(), pMeshEffect->Get_FileName().end());
			_uint iFileNameSize = pMeshEffect->Get_FileName().size() + 1;
			ofs.write((_char*)&iFileNameSize, sizeof(_uint));
			ofs.write(strName.c_str(), iFileNameSize);

			//파일 경로
			string strFilePath = "";
			strFilePath.assign(pMeshEffect->Get_TextureFilePath().begin(), pMeshEffect->Get_TextureFilePath().end());
			_uint iFilePathSize = pMeshEffect->Get_TextureFilePath().size() + 1;
			ofs.write((_char*)&iFilePathSize, sizeof(_uint));
			ofs.write(strFilePath.c_str(), iFilePathSize);

			//메쉬 이름
			string strMeshFileName = pMeshEffect->Get_ModelFileName();
			_uint iMeshFileNameSize = pMeshEffect->Get_ModelFileName().size() + 1;
			ofs.write((_char*)&iMeshFileNameSize, sizeof(_uint));
			ofs.write(strMeshFileName.c_str(), iMeshFileNameSize);

			//메쉬 파일 경로
			string strMeshFilePath = pMeshEffect->Get_MeshFilePath();
			_uint iMeshFilePathSize = strMeshFilePath.size() + 1;
			ofs.write((_char*)&iMeshFilePathSize, sizeof(_uint));
			ofs.write(strMeshFileName.c_str(), iMeshFilePathSize);

			//월드행렬
			const _float4x4* WorldMatrix = pMeshEffect->m_pTransformCom->Get_WorldFloat4x4();
			ofs.write((_char*)WorldMatrix, sizeof(_float4x4));
		}
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
	for (auto& pEffect : m_Effects)
	{
		_uint iEffectType = pEffect->Get_EffectType();
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
			Desc.iEffectType = iEffectType;

			//파티클 파일 이름
			_char strName[MAX_PATH] = "";
			_uint iFileNameSize = 0;
			ifs.read((_char*)&iFileNameSize, sizeof(_uint));
			ifs.read(strName, iFileNameSize);

			//파티클 파일 경로
			_char strFilePath[MAX_PATH] = "";
			_uint iFilePathSize = 0;
			ifs.read((_char*)&iFilePathSize, sizeof(_uint));
			ifs.read(strFilePath, iFilePathSize);

			//월드행렬
			_float4x4 WorldMatrix;
			ifs.read((_char*)&WorldMatrix, sizeof(_float4x4));

			string stringFileName = strName;
			wstring wstrFileName = L"";
			wstrFileName.assign(stringFileName.begin(), stringFileName.end());

			Desc.strTextureFileName = wstrFileName;
			strcpy_s(Desc.strTextureFilePath, strFilePath);

			Add_Effect(EFFECT_TYPE(iEffectType), &Desc);
		}
		else if (iEffectType == EFFECT_IMG)
		{
			CEffect_Default::EFFECT_DEFAULT_DESC Desc;
			//텍스쳐 파일 이름
			_char strName[MAX_PATH] = "";
			_uint iFileNameSize = 0;
			ifs.read((_char*)&iFileNameSize, sizeof(_uint));
			ifs.read(strName, iFileNameSize);

			//파일 경로
			_char strFilePath[MAX_PATH] = "";
			_uint iFilePathSize = 0;
			ifs.read((_char*)&iFilePathSize, sizeof(_uint));
			ifs.read(strFilePath, iFilePathSize);

			//월드행렬
			_float4x4 WorldMatrix;
			ifs.read((_char*)&WorldMatrix, sizeof(_float4x4));

			string stringFileName = strName;
			wstring wstrFileName = L"";
			wstrFileName.assign(stringFileName.begin(), stringFileName.end());

			Desc.strTextureFileName = wstrFileName;
			strcpy_s(Desc.strTextureFilePath, strFilePath);

			Add_Effect(EFFECT_TYPE(iEffectType), &Desc);
		}
		else if (iEffectType == EFFECT_MESH)
		{
			//파일 이름
			_char strName[MAX_PATH] = "";
			_uint iFileNameSize = 0;
			ifs.read((_char*)&iFileNameSize, sizeof(_uint));
			ifs.read(strName, iFileNameSize);

			//파일 경로
			_char strFilePath[MAX_PATH] = "";
			_uint iFilePathSize = 0;
			ifs.read((_char*)&iFilePathSize, sizeof(_uint));
			ifs.read(strFilePath, iFilePathSize);

			//메쉬 이름
			_char strMeshFileName[MAX_PATH] = "";
			_uint iMeshFileNameSize = 0;
			ifs.read((_char*)&iMeshFileNameSize, sizeof(_uint));
			ifs.read(strMeshFileName, iMeshFileNameSize);

			//메쉬 파일 경로
			_char strMeshFilePath[MAX_PATH] = "";
			_uint iMeshFilePathSize = 0;
			ifs.read((_char*)&iMeshFilePathSize, sizeof(_uint));
			ifs.read(strMeshFileName, iMeshFilePathSize);

			//월드행렬
			_float4x4 WorldMatrix;
			ifs.read((_char*)&WorldMatrix, sizeof(_float4x4));

			CEffect_Mesh::EFFECT_MESH_DESC Desc{};
			strcpy_s(Desc.szModelFilePath, strMeshFilePath); // 메쉬 경로
			strcpy_s(Desc.strTextureFilePath, strFilePath); // 텍스쳐 경로
			
			//텍스쳐 이름
			string stringFileName = strName;
			wstring wstrFileName = L"";
			wstrFileName.assign(stringFileName.begin(), stringFileName.end());
			Desc.strTextureFileName = wstrFileName;

			//메쉬 이름
			string stringMeshFileName = strMeshFileName;
			Desc.strModelFileName = stringMeshFileName;

			Add_Effect(EFFECT_TYPE(iEffectType), &Desc);
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
}
