#include "Effect_Trail.h"

#include "GameInstance.h"

CEffect_Trail::CEffect_Trail(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CEffectObject{ pDevice, pContext}
{
}

CEffect_Trail::CEffect_Trail(const CEffect_Trail& rhs)
	: CEffectObject{ rhs },
	m_strFilePath{ rhs.m_strFilePath },
	m_strMaskFilePath { rhs.m_strMaskFilePath},
	m_isMask { rhs.m_isMask }
{
	strcpy_s(m_szMeshFilePath, rhs.m_szMeshFilePath);
}

HRESULT CEffect_Trail::Initialize_Prototype(void* pArg)
{
	if (FAILED(__super::Initialize_Prototype(pArg)))
		return E_FAIL;

	EFFECT_TRAIL_DESC* pDesc = static_cast<EFFECT_TRAIL_DESC*>(pArg);
	string strFilePath = pDesc->strFilePath;
	string strMaskFilePath = pDesc->strMaskFilePath;
	m_strFilePath.assign(strFilePath.begin(), strFilePath.end());
	m_strMaskFilePath.assign(strMaskFilePath.begin(), strMaskFilePath.end());
	m_isMask = pDesc->isMask;

	strcpy_s(m_szMeshFilePath, pDesc->strMeshFilePath);

	return S_OK;
}

HRESULT CEffect_Trail::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Add_Components()))
		return E_FAIL;

	if (m_isTrailMove)
	{
		//플레이어 방향으로 보내기
		m_vPlayerLook = XMVector3Normalize(XMLoadFloat4x4(m_ParentMatrix).r[2]);
	}
	return S_OK;
}

void CEffect_Trail::Priority_Tick(const _float& fTimeDelta)
{
}

void CEffect_Trail::Tick(const _float& fTimeDelta)
{
	m_fTimeDelta += fTimeDelta * 2;

	// 수정하기 
	if (m_isTrailMove)
	{
		//플레이어 방향으로 이동하기
		_vector vPlayerPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
		vPlayerPos += m_vPlayerLook * fTimeDelta * m_fSpeed;
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPlayerPos);
	}
}

void CEffect_Trail::Late_Tick(const _float& fTimeDelta)
{
	XMStoreFloat4x4(&m_WorldMatrix, m_pTransformCom->Get_WorldMatrix() * XMLoadFloat4x4(m_ParentMatrix));
	
	m_pGameInstance->Add_Renderer(CRenderer::RENDER_NONLIGHT, this);
}

HRESULT CEffect_Trail::Render()
{
	if (FAILED(Bind_ResourceData()))
		return E_FAIL;

	_uint iNumMeshes = m_pModelCom->Get_NumMeshes();

	for (int i = 0; i < iNumMeshes; ++i)
	{
		m_pShaderCom->Begin(m_iShaderPass);
		m_pModelCom->Render(i);
	}

	return S_OK;
}

HRESULT CEffect_Trail::Add_Components()
{
	if (FAILED(Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Shader_VtxMesh_Effect", L"Com_Shader", reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	_matrix PreMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	m_pModelCom = CModel::Create(m_pDevice, m_pContext, m_szMeshFilePath, PreMatrix, m_szMeshFilePath);
	if (nullptr == m_pModelCom)
		return E_FAIL;

	m_pTextureCom = CTexture::Create(m_pDevice, m_pContext, m_strFilePath, 1);
	if (nullptr == m_pTextureCom)
		return E_FAIL;

	if (m_isMask)
	{
		m_pMaskTextureCom = CTexture::Create(m_pDevice, m_pContext, m_strMaskFilePath, 1);
		if (nullptr == m_pMaskTextureCom)
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CEffect_Trail::Bind_ResourceData()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_WorldMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture", 0)))
		return E_FAIL;

	//UV 조절하기
	if (FAILED(m_pShaderCom->Bind_RawValue("g_fTimeDelta", &m_fTimeDelta, sizeof(_float))))
		return E_FAIL;

	if (m_isMask)
	{
		if (FAILED(m_pMaskTextureCom->Bind_ShaderResource(m_pShaderCom, "g_MaskTexture", 0)))
			return E_FAIL;
	}

	return S_OK;
}

CEffect_Trail* CEffect_Trail::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, void* pArg)
{
	CEffect_Trail* pInstance = new CEffect_Trail(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
		Safe_Release(pInstance);

	return pInstance;
}

CGameObject* CEffect_Trail::Clone(void* pArg)
{
	CEffect_Trail* pInstance = new CEffect_Trail(*this);

	if (FAILED(pInstance->Initialize(pArg)))
		Safe_Release(pInstance);

	return pInstance;
}

void CEffect_Trail::Free()
{
	__super::Free();

	Safe_Release(m_pModelCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pMaskTextureCom);
}
