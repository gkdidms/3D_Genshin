#include "EffectObject.h"

#include "GameInstance.h"

CEffectObject::CEffectObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CBlendObject{ pDevice, pContext}
{
}

CEffectObject::CEffectObject(const CEffectObject& rhs)
	: CBlendObject { rhs },
	m_WorldMatrix{ rhs.m_WorldMatrix },
	m_iShaderPass{ rhs.m_iShaderPass },
	m_strTextureFilePath { rhs.m_strTextureFilePath },
	m_strMaskFilePath { rhs.m_strMaskFilePath},
	m_strNoiseFilePath { rhs.m_strNoiseFilePath},
	m_iTextureNum { rhs.m_iTextureNum},
	m_isMask {rhs.m_isMask},
	m_isNoise {rhs.m_isNoise},
	m_iEffectType { rhs.m_iEffectType},
	m_vColor {rhs.m_vColor},
	m_iRendererType {rhs.m_iRendererType},
	m_isFrameLoop { rhs.m_isFrameLoop},
	m_fStartTime { rhs.m_fStartTime},
	m_fDurationTime { rhs.m_fDurationTime}
{
}

HRESULT CEffectObject::Initialize_Prototype(void* pArg)
{
	if (nullptr == pArg)
		return E_FAIL;

	EFFECT_OBJECT_DESC* pDesc = static_cast<EFFECT_OBJECT_DESC*>(pArg);
	XMStoreFloat4x4(&m_WorldMatrix, pDesc->WorldMatrix);
	m_iShaderPass = pDesc->iShaderPass;
	
	string strTextureFilePath = pDesc->strTextureFilePath;
	m_strTextureFilePath.assign(strTextureFilePath.begin(), strTextureFilePath.end());

	string strMaskFilePath = pDesc->strMaskFilePath;
	m_strMaskFilePath.assign(strMaskFilePath.begin(), strMaskFilePath.end());

	string strNoiseFilePath = pDesc->strNoiseFilePath;
	m_strNoiseFilePath.assign(strNoiseFilePath.begin(), strNoiseFilePath.end());

	m_iTextureNum = pDesc->iTextureNum;
	m_isMask = pDesc->isMask;
	m_isNoise = pDesc->isNoise;
	m_iEffectType = pDesc->iEffectType;

	m_vColor = pDesc->vColor;
	m_iRendererType = pDesc->iRendererType;
	m_isFrameLoop = pDesc->isFrameLoop;
	m_fStartTime = pDesc->fStartTime;
	m_fDurationTime = pDesc->fDurationTime;

	return S_OK;
}

HRESULT CEffectObject::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	EFFECT_OBJECT_DESC* pDesc = static_cast<EFFECT_OBJECT_DESC*>(pArg);

	m_pTransformCom->Set_WorldMatrix(XMLoadFloat4x4(&m_WorldMatrix));
	m_ParentMatrix = pDesc->pParentMatrix;

	return S_OK;
}

void CEffectObject::Priority_Tick(const _float& fTimeDelta)
{
}

void CEffectObject::Tick(const _float& fTimeDelta)
{
}

void CEffectObject::Late_Tick(const _float& fTimeDelta)
{
}

HRESULT CEffectObject::Render()
{
	return S_OK;
}

HRESULT CEffectObject::Add_Components()
{
	m_pTextureCom = CTexture::Create(m_pDevice, m_pContext, m_strTextureFilePath, m_iTextureNum);
	if (nullptr == m_pTextureCom)
		return E_FAIL;

	if (m_strMaskFilePath.length() > 0 && m_isMask == true)
	{
		m_pMaskTextureCom = CTexture::Create(m_pDevice, m_pContext, m_strMaskFilePath, 1);
		if (nullptr == m_pMaskTextureCom)
			return E_FAIL;
	}

	if (m_strTextureFilePath.length() > 0 && m_isNoise == true)
	{
		m_pNoiseTextureCom = CTexture::Create(m_pDevice, m_pContext, m_strNoiseFilePath, 1);
		if (nullptr == m_pNoiseTextureCom)
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CEffectObject::Bind_ResourceData()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_WorldMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture", (_uint)m_fFrame)))
		return E_FAIL;

	if (m_isMask)
	{
		if (FAILED(m_pMaskTextureCom->Bind_ShaderResource(m_pShaderCom, "g_MaskTexture", 0)))
			return E_FAIL;
	}

	if (m_isNoise)
	{
		if (FAILED(m_pNoiseTextureCom->Bind_ShaderResource(m_pShaderCom, "g_NoiseTexture", 0)))
			return E_FAIL;
	}

	if (FAILED(m_pShaderCom->Bind_RawValue("g_vColor", &m_vColor, sizeof(_float4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_fTimeDelta", &m_fCurrentTime, sizeof(_float))))
		return E_FAIL;

	return S_OK;
}

void CEffectObject::Free()
{
	__super::Free();

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pMaskTextureCom);
	Safe_Release(m_pNoiseTextureCom);
}
