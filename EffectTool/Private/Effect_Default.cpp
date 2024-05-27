#include "Effect_Default.h"

#include "GameInstance.h"

CEffect_Default::CEffect_Default(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CTool_Effect{ pDevice, pContext }
{
}

CEffect_Default::CEffect_Default(const CEffect_Default& rhs)
	: CTool_Effect { rhs }
{
}

HRESULT CEffect_Default::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CEffect_Default::Initialize(void* pArg)
{
	if (nullptr == pArg)
		return E_FAIL;

	EFFECT_DEFAULT_DESC* pDesc = static_cast<EFFECT_DEFAULT_DESC*>(pArg);
	string strTextureFilePath(pDesc->strTextureFilePath);
	m_strTextureFilePath.assign(strTextureFilePath.begin(), strTextureFilePath.end());

	string strTrailFilePath(pDesc->strTrailFilePath);
	m_strTrailTextureFilePath.assign(strTrailFilePath.begin(), strTrailFilePath.end());
	m_isTrailBuffer = pDesc->isTrailBuffer;
	m_iNumTexture = pDesc->iNumTexture;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Add_Components()))
		return E_FAIL;

	return S_OK;
}

void CEffect_Default::Priority_Tick(const _float& fTimeDelta)
{
}

void CEffect_Default::Tick(const _float& fTimeDelta)
{
	if (m_isStart)
	{
		m_fCurrentTime += fTimeDelta;

		if (m_fCurrentTime >= m_fDuration)
		{
			m_isStart = false;
			m_fCurrentTime = 0.f;
			return;
		}

		m_pTransformCom->Go_Straight(fTimeDelta);
	}

	m_pTransformCom->LookAt(XMVectorSet(0.f, 0.f, 100.f, 1.f));

	if (m_isTrailBuffer) m_pTrailVIBufferCom->Add_Trail(fTimeDelta, m_pTransformCom->Get_WorldMatrix());
}

void CEffect_Default::Late_Tick(const _float& fTimeDelta)
{
	m_pGameInstance->Add_Renderer(CRenderer::RENDER_NONBLENDER, this);
}

HRESULT CEffect_Default::Render()
{
	if (FAILED(Bind_ResourceData()))
		return E_FAIL;

	m_pShaderCom->Begin(m_iShaderPass);
	m_pVIBufferCom->Render();

	if (m_isTrailBuffer)
	{
		m_pShaderCom->Begin(1);
		m_pTrailVIBufferCom->Render();
	}

	return S_OK;
}

HRESULT CEffect_Default::Add_Components()
{
	if (FAILED(Add_Component(LEVEL_MAIN, L"Prototype_Component_Shader_VtxPosTex", L"Com_Shader", reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	
	if (m_isTrailBuffer)
	{
		if (FAILED(Add_Component(LEVEL_MAIN, L"Prototype_Component_VIBuffer_RectZ", L"Com_VIBuffer", reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
			return E_FAIL;

		m_pTrailTextureCom = CTexture::Create(m_pDevice, m_pContext, m_strTrailTextureFilePath, 1);
		if (nullptr == m_pTrailTextureCom)
			return E_FAIL;
	}
	else
	{
		if (FAILED(Add_Component(LEVEL_MAIN, L"Prototype_Component_VIBuffer_Rect", L"Com_VIBuffer", reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
			return E_FAIL;
	}

	if (FAILED(Add_Component(LEVEL_MAIN, L"Prototype_Component_VIBuffer_Trail", L"Com_TrailBuffer", reinterpret_cast<CComponent**>(&m_pTrailVIBufferCom))))
		return E_FAIL;

	m_pTextureCom = CTexture::Create(m_pDevice, m_pContext, m_strTextureFilePath, m_iNumTexture);
	if (nullptr == m_pTextureCom)
		return E_FAIL;

	return S_OK;
}

HRESULT CEffect_Default::Bind_ResourceData()
{
	if (FAILED(m_pTransformCom->Bind_ShaderMatrix(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", 0)))
		return E_FAIL;

	if (m_isTrailBuffer)
	{
		if (FAILED(m_pTrailTextureCom->Bind_ShaderResource(m_pShaderCom, "g_TrailTexture", 0)))
			return E_FAIL;
	}

	return S_OK;
}

CEffect_Default* CEffect_Default::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CEffect_Default* pInstance = new CEffect_Default(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
		Safe_Release(pInstance);

	return pInstance;
}

CGameObject* CEffect_Default::Clone(void* pArg)
{
	CEffect_Default* pInstance = new CEffect_Default(*this);

	if (FAILED(pInstance->Initialize(pArg)))
		Safe_Release(pInstance);

	return pInstance;
}

void CEffect_Default::Free()
{
	__super::Free();

	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTrailVIBufferCom);
	Safe_Release(m_pTrailTextureCom);
}
